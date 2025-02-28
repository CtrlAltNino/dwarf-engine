#include <glad/glad.h>
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "OpenGLFramebuffer.h"

#include "Platform/OpenGL/OpenGLRendererApi.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Platform/OpenGL/OpenGLMesh.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLComputeShader.h"
#include "Platform/OpenGL/OpenGLUtilities.h"

namespace Dwarf
{
  OpenGLRendererApi::OpenGLRendererApi(
    std::shared_ptr<IAssetDatabase>      assetDatabase,
    std::shared_ptr<IShaderFactory>      shaderFactory,
    std::shared_ptr<IDwarfLogger>        logger,
    std::shared_ptr<IEditorStats>        editorStats,
    std::shared_ptr<IOpenGLStateTracker> stateTracker)
    : m_AssetDatabase(assetDatabase)
    , m_ShaderFactory(shaderFactory)
    , m_Logger(logger)
    , m_EditorStats(editorStats)
    , m_StateTracker(stateTracker)
  {
    m_Logger->LogDebug(Log("OpenGLRendererApi created.", "OpenGLRendererApi"));
    m_ErrorShader = m_ShaderFactory->CreateErrorShader();
    m_ErrorShader->Compile();

    m_StateTracker->SetDepthTest(true);
    m_StateTracker->SetDepthFunction(GL_LESS);
    glEnable(GL_LINE_SMOOTH);
    OpenGLUtilities::CheckOpenGLError(
      "glEnable GL_LINE_SMOOTH", "OpenGLRendererApi", m_Logger);
  }

  OpenGLRendererApi::~OpenGLRendererApi()
  {
    m_Logger->LogDebug(
      Log("OpenGLRendererApi destroyed.", "OpenGLRendererApi"));
  }

  struct SetShaderParameterVisitor
  {
    OpenGLShader&                   m_Shader;
    std::string                     m_ParameterName;
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;
    std::shared_ptr<IDwarfLogger>   m_Logger;
    unsigned int&                   m_TextureCount;

    void
    operator()(bool& parameter)
    {
      m_Shader.SetUniform(m_ParameterName, parameter);
    }
    void
    operator()(int& parameter)
    {
      m_Shader.SetUniform(m_ParameterName, parameter);
    }
    void
    operator()(unsigned int& parameter)
    {
      m_Shader.SetUniform(m_ParameterName, parameter);
    }
    void
    operator()(float& parameter)
    {
      m_Shader.SetUniform(m_ParameterName, parameter);
    }
    void
    operator()(Texture2DAssetValue& parameter)
    {
      if (parameter.has_value() && m_AssetDatabase->Exists(parameter.value()))
      {
        TextureAsset& textureAsset =
          (TextureAsset&)m_AssetDatabase->Retrieve(*parameter)->GetAsset();
        m_Shader.SetUniform(m_ParameterName, textureAsset, m_TextureCount++);
      }
    }
    void
    operator()(glm::vec2& parameter)
    {
      m_Shader.SetUniform(m_ParameterName, parameter);
    }
    void
    operator()(glm::vec3& parameter)
    {
      m_Shader.SetUniform(m_ParameterName, parameter);
    }
    void
    operator()(glm::vec4& parameter)
    {
      m_Shader.SetUniform(m_ParameterName, parameter);
    }
  };

  void
  OpenGLRendererApi::SetViewport(uint32_t x,
                                 uint32_t y,
                                 uint32_t width,
                                 uint32_t height)
  {
    m_StateTracker->SetViewport(x, y, width, height);
  }

  void
  OpenGLRendererApi::SetClearColor(const glm::vec4& color)
  {
    m_StateTracker->SetClearColor(color);
  }

  void
  OpenGLRendererApi::Clear()
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before clearing", "OpenGLRendererApi", m_Logger);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    OpenGLUtilities::CheckOpenGLError("glClear", "OpenGLRendererApi", m_Logger);
  }

  void
  OpenGLRendererApi::Clear(unsigned int value)
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before clearing", "OpenGLRendererApi", m_Logger);
    glClearBufferuiv(
      GL_COLOR, 0, &value); // Use glClearBufferuiv for integer types
    OpenGLUtilities::CheckOpenGLError(
      "glClearBufferuiv", "OpenGLRendererApi", m_Logger);
    glClear(GL_DEPTH_BUFFER_BIT);
    OpenGLUtilities::CheckOpenGLError("glClear", "OpenGLRendererApi", m_Logger);
  }

  void
  OpenGLRendererApi::RenderIndexed(IMeshBuffer& mesh,
                                   IMaterial&   material,
                                   ICamera&     camera,
                                   glm::mat4    modelMatrix)
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before rendering", "OpenGLRendererApi", m_Logger);
    OpenGLMesh&   oglMesh = (OpenGLMesh&)mesh;
    IShader&      baseShader = material.GetShader();
    OpenGLShader& shader = baseShader.IsCompiled()
                             ? dynamic_cast<OpenGLShader&>(baseShader)
                             : dynamic_cast<OpenGLShader&>(*m_ErrorShader);
    int           textureInputCounter = 0;

    m_StateTracker->SetShaderProgram(shader.GetID());

    m_StateTracker->SetBlendMode(
      material.GetMaterialProperties().IsTransparent);

    m_StateTracker->SetBlendFunction(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_StateTracker->SetCullMode(
      !material.GetMaterialProperties().IsDoubleSided);
    m_StateTracker->SetCullFace(GL_BACK);

    // TODO: Move this to OpenGLShader.cpp
    unsigned int textureCount = 0;
    for (auto const& identifier :
         material.GetShaderParameters()->GetParameterIdentifiers())
    {
      if (material.GetShaderParameters()->HasParameter(identifier))
      {
        ParameterValue& shaderParameterValue =
          material.GetShaderParameters()->GetParameter(identifier);
        std::visit(
          SetShaderParameterVisitor{
            shader, identifier, m_AssetDatabase, m_Logger, textureCount },
          shaderParameterValue);
      }
    }

    shader.SetUniform("modelMatrix", modelMatrix);
    shader.SetUniform("viewMatrix", camera.GetViewMatrix());
    shader.SetUniform("projectionMatrix", camera.GetProjectionMatrix());
    shader.SetUniform("_Time", (float)m_EditorStats->GetTimeSinceStart());
    shader.SetUniform("viewPosition",
                      camera.GetProperties().Transform.GetPosition());

    oglMesh.Bind();

    glDrawElements(GL_TRIANGLES, oglMesh.GetIndexCount(), GL_UNSIGNED_INT, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glDrawElements", "OpenGLRendererApi", m_Logger);
  }

  void
  OpenGLRendererApi::ApplyComputeShader(
    std::shared_ptr<IComputeShader> computeShader,
    std::shared_ptr<IFramebuffer>   fb,
    uint32_t                        sourceAttachment,
    uint32_t                        destinationAttachment)
  {
    std::shared_ptr<OpenGLComputeShader> shader =
      std::dynamic_pointer_cast<OpenGLComputeShader>(computeShader);
    glUseProgram(shader->GetID());
    glBindImageTexture(
      0,
      fb->GetColorAttachment(sourceAttachment).value().get().GetTextureID(),
      0,
      GL_FALSE,
      0,
      GL_READ_ONLY,
      GL_RGBA8);
    glBindImageTexture(1,
                       fb->GetColorAttachment(destinationAttachment)
                         .value()
                         .get()
                         .GetTextureID(),
                       0,
                       GL_FALSE,
                       0,
                       GL_WRITE_ONLY,
                       GL_RGBA8);

    glDispatchCompute(
      fb->GetSpecification().Width / 16, fb->GetSpecification().Height / 16, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glUseProgram(0);
  }

  void
  OpenGLRendererApi::Blit(IFramebuffer& source,
                          IFramebuffer& destination,
                          uint32_t      sourceAttachment,
                          uint32_t      destinationAttachment,
                          uint32_t      width,
                          uint32_t      height)
  {
    OpenGLFramebuffer* sourceFB = (OpenGLFramebuffer*)&source;
    OpenGLFramebuffer* destinationFB = (OpenGLFramebuffer*)&destination;
    glBindFramebuffer(GL_READ_FRAMEBUFFER,
                      sourceFB->GetFramebufferRendererID());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,
                      destinationFB->GetFramebufferRendererID());
    glBlitFramebuffer(0,
                      0,
                      width,
                      height,
                      0,
                      0,
                      width,
                      height,
                      GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  VRAMUsageBuffer
  OpenGLRendererApi::QueryVRAMUsage()
  {
    VRAMUsageBuffer result;

    if (GLAD_GL_NVX_gpu_memory_info)
    {
      GLint totalMemoryKb = 0, availableMemoryKb = 0;
      glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX,
                    &totalMemoryKb);
      glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX,
                    &availableMemoryKb);

      result.totalMemoryMb = totalMemoryKb / 1024;
      result.usedMemoryMb = (totalMemoryKb - availableMemoryKb) / 1024;
    }
    else
    {
      // std::cout << "GL_NVX_gpu_memory_info extension not supported.\n";
    }

    return result;
  }

  int32_t
  OpenGLRendererApi::GetMaxSamples()
  {
    GLint maxSamples = 0;
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);

    return maxSamples;
  }
}