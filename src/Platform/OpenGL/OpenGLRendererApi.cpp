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
    std::shared_ptr<IAssetDatabase> assetDatabase,
    std::shared_ptr<IShaderFactory> shaderFactory,
    std::shared_ptr<IDwarfLogger>   logger,
    std::shared_ptr<IEditorStats>   editorStats)
    : m_AssetDatabase(assetDatabase)
    , m_ShaderFactory(shaderFactory)
    , m_Logger(logger)
    , m_EditorStats(editorStats)
  {
    m_Logger->LogDebug(Log("OpenGLRendererApi created.", "OpenGLRendererApi"));
    m_ErrorShader = m_ShaderFactory->CreateErrorShader();
    m_ErrorShader->Compile();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
  }

  OpenGLRendererApi::~OpenGLRendererApi()
  {
    m_Logger->LogDebug(
      Log("OpenGLRendererApi destroyed.", "OpenGLRendererApi"));
  }

  struct SetShaderParameterVisitor
  {
    GLuint                          m_ShaderID;
    std::string                     m_ParameterName;
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;
    std::shared_ptr<IDwarfLogger>   m_Logger;
    unsigned int&                   m_TextureCount;

    void
    operator()(bool& parameter)
    {
      glUniform1f(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter);
      OpenGLUtilities::CheckOpenGLError(
        "glUniform1f", "OpenGLRendererApi", m_Logger);
    }
    void
    operator()(int& parameter)
    {
      glUniform1i(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter);
      OpenGLUtilities::CheckOpenGLError(
        "glUniform1i", "OpenGLRendererApi", m_Logger);
    }
    void
    operator()(unsigned int& parameter)
    {
      glUniform1ui(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                   parameter);
      OpenGLUtilities::CheckOpenGLError(
        "glUniform1ui", "OpenGLRendererApi", m_Logger);
    }
    void
    operator()(float& parameter)
    {
      glUniform1f(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter);
      OpenGLUtilities::CheckOpenGLError(
        "glUniform1f", "OpenGLRendererApi", m_Logger);
    }
    void
    operator()(Texture2DAssetValue& parameter)
    {
      // m_Logger->LogDebug(
      //   Log(fmt::format("Trying to bind texture to {}", m_ParameterName),
      //       "OpenGLRendererApi"));
      if (parameter.has_value() && m_AssetDatabase->Exists(parameter.value()))
      {
        // m_Logger->LogDebug(Log("Texture found", "OpenGLRendererApi"));
        // m_Logger->LogDebug(Log(fmt::format("Texture count: {}",
        // m_TextureCount),
        //                        "OpenGLRendererApi"));

        // TODO: This needs to count the number of textures and bind them
        glActiveTexture(GL_TEXTURE0 + m_TextureCount);
        OpenGLUtilities::CheckOpenGLError(
          "glActiveTexture", "OpenGLRendererApi", m_Logger);
        TextureAsset& textureAsset =
          (TextureAsset&)m_AssetDatabase->Retrieve(*parameter)->GetAsset();
        glBindTexture(GL_TEXTURE_2D, textureAsset.GetTexture().GetTextureID());
        // m_Logger->LogDebug(
        //   Log(fmt::format("Texture id: {}",
        //                   textureAsset.GetTexture().GetTextureID()),
        //       "OpenGLRendererApi"));
        OpenGLUtilities::CheckOpenGLError(
          "glBindTexture", "OpenGLRendererApi", m_Logger);
        GLint location =
          glGetUniformLocation(m_ShaderID, m_ParameterName.c_str());
        OpenGLUtilities::CheckOpenGLError(
          "glGetUniformLocation", "OpenGLRendererApi", m_Logger);
        if (location == -1)
        {
          m_Logger->LogError(
            Log(fmt::format("Uniform location <{}> not found in shader program",
                            m_ParameterName),
                "OpenGLRendererApi"));
        }
        else
        {
          glUniform1i(location, static_cast<GLint>(m_TextureCount));
          OpenGLUtilities::CheckOpenGLError(
            "glUniform1i", "OpenGLRendererApi", m_Logger);
          m_TextureCount++;
        }
      }
    }
    void
    operator()(glm::vec2& parameter)
    {
      glUniform2f(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter.x,
                  parameter.y);
      OpenGLUtilities::CheckOpenGLError(
        "glUniform2f", "OpenGLRendererApi", m_Logger);
    }
    void
    operator()(glm::vec3& parameter)
    {
      glUniform3f(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter.x,
                  parameter.y,
                  parameter.z);
      OpenGLUtilities::CheckOpenGLError(
        "glUniform3f", "OpenGLRendererApi", m_Logger);
    }
    void
    operator()(glm::vec4& parameter)
    {
      glUniform4f(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter.x,
                  parameter.y,
                  parameter.z,
                  parameter.w);
      OpenGLUtilities::CheckOpenGLError(
        "glUniform4f", "OpenGLRendererApi", m_Logger);
    }
  };

  void
  OpenGLRendererApi::Init()
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before OpenGLRendererApi::Init", "OpenGLRendererApi", m_Logger);
    glEnable(GL_BLEND);
    OpenGLUtilities::CheckOpenGLError(
      "glEnable GL_BLEND", "OpenGLRendererApi", m_Logger);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    OpenGLUtilities::CheckOpenGLError(
      "glBlendFunc", "OpenGLRendererApi", m_Logger);

    glEnable(GL_DEPTH_TEST);
    OpenGLUtilities::CheckOpenGLError(
      "glEnable GL_DEPTH_TEST", "OpenGLRendererApi", m_Logger);
    glEnable(GL_LINE_SMOOTH);
    OpenGLUtilities::CheckOpenGLError(
      "glEnable GL_LINE_SMOOTH", "OpenGLRendererApi", m_Logger);
    SetViewport(0, 0, 512, 512);
  }

  void
  OpenGLRendererApi::SetViewport(uint32_t x,
                                 uint32_t y,
                                 uint32_t width,
                                 uint32_t height)
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before setting viewport", "OpenGLRendererApi", m_Logger);
    glViewport(x, y, width, height);
    OpenGLUtilities::CheckOpenGLError(
      "glViewport", "OpenGLRendererApi", m_Logger);
  }

  void
  OpenGLRendererApi::SetClearColor(const glm::vec4& color)
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before setting clear color", "OpenGLRendererApi", m_Logger);
    glClearColor(color.r, color.g, color.b, color.a);
    OpenGLUtilities::CheckOpenGLError(
      "glClearColor", "OpenGLRendererApi", m_Logger);
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
  OpenGLRendererApi::RenderIndexed(std::unique_ptr<IMesh>& mesh,
                                   IMaterial&              material,
                                   ICamera&                camera,
                                   glm::mat4               modelMatrix)
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before rendering", "OpenGLRendererApi", m_Logger);
    OpenGLMesh*   oglMesh = (OpenGLMesh*)mesh.get();
    IShader&      baseShader = material.GetShader();
    OpenGLShader& shader = baseShader.IsCompiled()
                             ? dynamic_cast<OpenGLShader&>(baseShader)
                             : dynamic_cast<OpenGLShader&>(*m_ErrorShader);
    int           textureInputCounter = 0;

    static GLuint currentShaderId = -1;

    if (currentShaderId != shader.GetID())
    {
      currentShaderId = shader.GetID();
      glUseProgram(currentShaderId);
      OpenGLUtilities::CheckOpenGLError(
        "glUseProgram", "OpenGLRendererApi", m_Logger);
    }

    static bool transparentFlag = false;

    if (transparentFlag != material.GetMaterialProperties().IsTransparent)
    {
      transparentFlag = material.GetMaterialProperties().IsTransparent;
      if (transparentFlag)
      {
        glEnable(GL_BLEND);
        OpenGLUtilities::CheckOpenGLError(
          "glEnable GL_BLEND", "OpenGLRendererApi", m_Logger);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        OpenGLUtilities::CheckOpenGLError(
          "glBlendFunc", "OpenGLRendererApi", m_Logger);
      }
      else
      {
        glDisable(GL_BLEND);
        OpenGLUtilities::CheckOpenGLError(
          "glEnable GL_BLEND", "OpenGLRendererApi", m_Logger);
      }
    }

    static bool cullFaceFlag = true;

    if (cullFaceFlag != !material.GetMaterialProperties().IsDoubleSided)
    {
      cullFaceFlag = !material.GetMaterialProperties().IsDoubleSided;

      if (cullFaceFlag)
      {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        OpenGLUtilities::CheckOpenGLError(
          "glCullFace", "OpenGLRendererApi", m_Logger);
      }
      else
      {
        glDisable(GL_CULL_FACE);
      }
    }

    // TODO: Move this to OpenGLShader.cpp
    unsigned int textureCount = 0;
    for (auto const& identifier :
         material.GetShaderParameters()->GetParameterIdentifiers())
    {
      if (material.GetShaderParameters()->HasParameter(identifier))
      {
        ParameterValue& shaderParameterValue =
          material.GetShaderParameters()->GetParameter(identifier);
        std::visit(SetShaderParameterVisitor{ shader.GetID(),
                                              identifier,
                                              m_AssetDatabase,
                                              m_Logger,
                                              textureCount },
                   shaderParameterValue);
      }
    }

    GLuint mmID = glGetUniformLocation(shader.GetID(), "modelMatrix");
    OpenGLUtilities::CheckOpenGLError(
      "glGetUniformLocation modelMatrix", "OpenGLRendererApi", m_Logger);
    GLuint vmID = glGetUniformLocation(shader.GetID(), "viewMatrix");
    OpenGLUtilities::CheckOpenGLError(
      "glGetUniformLocation viewMatrix", "OpenGLRendererApi", m_Logger);
    GLuint pmID = glGetUniformLocation(shader.GetID(), "projectionMatrix");
    OpenGLUtilities::CheckOpenGLError(
      "glGetUniformLocation projectionMatrix", "OpenGLRendererApi", m_Logger);
    GLuint timeID = glGetUniformLocation(shader.GetID(), "_Time");
    OpenGLUtilities::CheckOpenGLError(
      "glGetUniformLocation time", "OpenGLRendererApi", m_Logger);
    GLuint vpID = glGetUniformLocation(shader.GetID(), "viewPosition");
    OpenGLUtilities::CheckOpenGLError(
      "glGetUniformLocation viewPosition", "OpenGLRendererApi", m_Logger);

    glUniformMatrix4fv(mmID, 1, GL_FALSE, &modelMatrix[0][0]);
    OpenGLUtilities::CheckOpenGLError(
      "glUniformMatrix4fv modelMatrix", "OpenGLRendererApi", m_Logger);
    glUniformMatrix4fv(vmID, 1, GL_FALSE, &camera.GetViewMatrix()[0][0]);
    OpenGLUtilities::CheckOpenGLError(
      "glUniformMatrix4fv viewMatrix", "OpenGLRendererApi", m_Logger);
    glUniformMatrix4fv(pmID, 1, GL_FALSE, &camera.GetProjectionMatrix()[0][0]);
    OpenGLUtilities::CheckOpenGLError(
      "glUniformMatrix4fv projectionMatrix", "OpenGLRendererApi", m_Logger);
    glUniform1f(timeID, (float)m_EditorStats->GetTimeSinceStart());
    OpenGLUtilities::CheckOpenGLError(
      "glUniform1f time", "OpenGLRendererApi", m_Logger);
    glUniform3f(vpID,
                camera.GetProperties().Transform.GetPosition().x,
                camera.GetProperties().Transform.GetPosition().y,
                camera.GetProperties().Transform.GetPosition().z);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform3f viewPosition", "OpenGLRendererApi", m_Logger);

    oglMesh->Bind();

    glDrawElements(
      GL_TRIANGLES, oglMesh->GetIndices().size(), GL_UNSIGNED_INT, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glDrawElements", "OpenGLRendererApi", m_Logger);

    oglMesh->Unbind();

    glDisable(GL_BLEND);
    OpenGLUtilities::CheckOpenGLError(
      "glDisable GL_BLEND", "OpenGLRendererApi", m_Logger);
    glDisable(GL_CULL_FACE);
    OpenGLUtilities::CheckOpenGLError(
      "glDisable GL_CULL_FACE", "OpenGLRendererApi", m_Logger);
    // glUseProgram(0);
    // OpenGLUtilities::CheckOpenGLError(
    //  "glUseProgram 0", "OpenGLRendererApi", m_Logger);
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