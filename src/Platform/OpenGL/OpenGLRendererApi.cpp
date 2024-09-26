#include <glad/glad.h>
#include "Core/Asset/Database/IAssetDatabase.h"
#include "OpenGLFramebuffer.h"

#include "Platform/OpenGL/OpenGLRendererApi.h"
#include "Platform/OpenGL/OpenGLMesh.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLComputeShader.h"
#include "Platform/OpenGL/OpenGLUtilities.h"

namespace Dwarf
{
  OpenGLRendererApi::OpenGLRendererApi(
    std::shared_ptr<IAssetDatabase> assetDatabase,
    std::shared_ptr<IShaderFactory> shaderFactory,
    std::shared_ptr<IDwarfLogger>   logger)
    : m_AssetDatabase(assetDatabase)
    , m_ShaderFactory(shaderFactory)
    , m_Logger(logger)
  {
    m_Logger->LogInfo(Log("OpenGLRendererApi created.", "OpenGLRendererApi"));
    m_ErrorShader = m_ShaderFactory->CreateErrorShader();
    m_ErrorShader->Compile();
  }

  OpenGLRendererApi::~OpenGLRendererApi()
  {
    m_Logger->LogInfo(Log("OpenGLRendererApi destroyed.", "OpenGLRendererApi"));
  }

  struct SetShaderParameterVisitor
  {
    GLuint                          m_ShaderID;
    std::string                     m_ParameterName;
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;
    std::shared_ptr<IDwarfLogger>   m_Logger;

    void
    operator()(bool& parameter)
    {
      glUniform1f(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter);
      OpenGLUtilities::CheckOpenGLError("glUniform1f", m_Logger);
    }
    void
    operator()(int& parameter)
    {
      glUniform1i(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter);
      OpenGLUtilities::CheckOpenGLError("glUniform1i", m_Logger);
    }
    void
    operator()(unsigned int& parameter)
    {
      glUniform1ui(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                   parameter);
      OpenGLUtilities::CheckOpenGLError("glUniform1ui", m_Logger);
    }
    void
    operator()(float& parameter)
    {
      glUniform1f(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter);
      OpenGLUtilities::CheckOpenGLError("glUniform1f", m_Logger);
    }
    void
    operator()(Texture2DAssetValue& parameter)
    {
      // TODO: This needs to count the number of textures and bind them
      glActiveTexture(GL_TEXTURE0);
      OpenGLUtilities::CheckOpenGLError("glActiveTexture", m_Logger);
      TextureAsset& textureAsset =
        (TextureAsset&)m_AssetDatabase->Retrieve(*parameter)->GetAsset();
      glBindTexture(GL_TEXTURE_2D, textureAsset.GetTexture().GetTextureID());
      OpenGLUtilities::CheckOpenGLError("glBindTexture", m_Logger);
      glUniform1i(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()), 0);
      OpenGLUtilities::CheckOpenGLError("glUniform1i", m_Logger);
    }
    void
    operator()(glm::vec2& parameter)
    {
      glUniform2f(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter.x,
                  parameter.y);
      OpenGLUtilities::CheckOpenGLError("glUniform2f", m_Logger);
    }
    void
    operator()(glm::vec3& parameter)
    {
      glUniform3f(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter.x,
                  parameter.y,
                  parameter.z);
      OpenGLUtilities::CheckOpenGLError("glUniform3f", m_Logger);
    }
    void
    operator()(glm::vec4& parameter)
    {
      glUniform4f(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter.x,
                  parameter.y,
                  parameter.z,
                  parameter.w);
      OpenGLUtilities::CheckOpenGLError("glUniform4f", m_Logger);
    }
  };

  void
  OpenGLRendererApi::Init()
  {
    OpenGLUtilities::CheckOpenGLError("Before OpenGLRendererApi::Init",
                                      m_Logger);
    glEnable(GL_BLEND);
    OpenGLUtilities::CheckOpenGLError("glEnable GL_BLEND", m_Logger);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    OpenGLUtilities::CheckOpenGLError("glBlendFunc", m_Logger);

    glEnable(GL_DEPTH_TEST);
    OpenGLUtilities::CheckOpenGLError("glEnable GL_DEPTH_TEST", m_Logger);
    glEnable(GL_LINE_SMOOTH);
    OpenGLUtilities::CheckOpenGLError("glEnable GL_LINE_SMOOTH", m_Logger);
    SetViewport(0, 0, 512, 512);
  }

  void
  OpenGLRendererApi::SetViewport(uint32_t x,
                                 uint32_t y,
                                 uint32_t width,
                                 uint32_t height)
  {
    OpenGLUtilities::CheckOpenGLError("Before setting viewport", m_Logger);
    glViewport(x, y, width, height);
    OpenGLUtilities::CheckOpenGLError("glViewport", m_Logger);
  }

  void
  OpenGLRendererApi::SetClearColor(const glm::vec4& color)
  {
    OpenGLUtilities::CheckOpenGLError("Before setting clear color", m_Logger);
    glClearColor(color.r, color.g, color.b, color.a);
    OpenGLUtilities::CheckOpenGLError("glClearColor", m_Logger);
  }

  void
  OpenGLRendererApi::Clear()
  {
    OpenGLUtilities::CheckOpenGLError("Before clearing", m_Logger);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    OpenGLUtilities::CheckOpenGLError("glClear", m_Logger);
  }

  void
  OpenGLRendererApi::Clear(unsigned int value)
  {
    OpenGLUtilities::CheckOpenGLError("Before clearing", m_Logger);
    glClearBufferuiv(
      GL_COLOR, 0, &value); // Use glClearBufferuiv for integer types
    OpenGLUtilities::CheckOpenGLError("glClearBufferuiv", m_Logger);
    glClear(GL_DEPTH_BUFFER_BIT);
    OpenGLUtilities::CheckOpenGLError("glClear", m_Logger);
  }

  void
  OpenGLRendererApi::RenderIndexed(std::unique_ptr<IMesh>& mesh,
                                   IMaterial&              material,
                                   glm::mat4               modelMatrix,
                                   glm::mat4               viewMatrix,
                                   glm::mat4               projectionMatrix)
  {
    OpenGLUtilities::CheckOpenGLError("Before rendering", m_Logger);
    OpenGLMesh*   oglMesh = (OpenGLMesh*)mesh.get();
    OpenGLShader* shader = material.GetShader()->IsCompiled()
                             ? (OpenGLShader*)material.GetShader().get()
                             : (OpenGLShader*)m_ErrorShader.get();
    char          textureInputCounter = 0;

    glUseProgram(shader->GetID());
    OpenGLUtilities::CheckOpenGLError("glUseProgram", m_Logger);

    if (material.GetMaterialProperties().IsTransparent)
    {
      glEnable(GL_BLEND);
      OpenGLUtilities::CheckOpenGLError("glEnable GL_BLEND", m_Logger);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      OpenGLUtilities::CheckOpenGLError("glBlendFunc", m_Logger);
    }

    glEnable(GL_CULL_FACE);
    OpenGLUtilities::CheckOpenGLError("glEnable GL_CULL_FACE", m_Logger);
    glCullFace(GL_BACK);
    OpenGLUtilities::CheckOpenGLError("glCullFace", m_Logger);

    // TODO: Move this to OpenGLShader.cpp
    for (auto const& identifier :
         material.GetShaderParameters()->GetParameterIdentifiers())
    {
      if (material.GetShaderParameters()->HasParameter(identifier))
      {
        ParameterValue& shaderParameterValue =
          material.GetShaderParameters()->GetParameter(identifier);
        std::visit(SetShaderParameterVisitor{ shader->GetID(), identifier },
                   shaderParameterValue);
      }
    }

    GLuint mmID = glGetUniformLocation(shader->GetID(), "modelMatrix");
    OpenGLUtilities::CheckOpenGLError("glGetUniformLocation modelMatrix",
                                      m_Logger);
    GLuint vmID = glGetUniformLocation(shader->GetID(), "viewMatrix");
    OpenGLUtilities::CheckOpenGLError("glGetUniformLocation viewMatrix",
                                      m_Logger);
    GLuint pmID = glGetUniformLocation(shader->GetID(), "projectionMatrix");
    OpenGLUtilities::CheckOpenGLError("glGetUniformLocation projectionMatrix",
                                      m_Logger);

    glUniformMatrix4fv(mmID, 1, GL_FALSE, &modelMatrix[0][0]);
    OpenGLUtilities::CheckOpenGLError("glUniformMatrix4fv modelMatrix",
                                      m_Logger);
    glUniformMatrix4fv(vmID, 1, GL_FALSE, &viewMatrix[0][0]);
    OpenGLUtilities::CheckOpenGLError("glUniformMatrix4fv viewMatrix",
                                      m_Logger);
    glUniformMatrix4fv(pmID, 1, GL_FALSE, &projectionMatrix[0][0]);
    OpenGLUtilities::CheckOpenGLError("glUniformMatrix4fv projectionMatrix",
                                      m_Logger);
    oglMesh->Bind();

    glDrawElements(
      GL_TRIANGLES, oglMesh->GetIndices().size(), GL_UNSIGNED_INT, nullptr);
    OpenGLUtilities::CheckOpenGLError("glDrawElements", m_Logger);

    oglMesh->Unbind();

    glDisable(GL_BLEND);
    OpenGLUtilities::CheckOpenGLError("glDisable GL_BLEND", m_Logger);
    glDisable(GL_CULL_FACE);
    OpenGLUtilities::CheckOpenGLError("glDisable GL_CULL_FACE", m_Logger);
    glUseProgram(0);
    OpenGLUtilities::CheckOpenGLError("glUseProgram 0", m_Logger);
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
  OpenGLRendererApi::Blit(std::shared_ptr<IFramebuffer> source,
                          std::shared_ptr<IFramebuffer> destination,
                          uint32_t                      sourceAttachment,
                          uint32_t                      destinationAttachment,
                          uint32_t                      width,
                          uint32_t                      height)
  {
    glBindFramebuffer(GL_READ_FRAMEBUFFER,
                      std::dynamic_pointer_cast<OpenGLFramebuffer>(source)
                        ->GetFramebufferRendererID());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,
                      std::dynamic_pointer_cast<OpenGLFramebuffer>(destination)
                        ->GetFramebufferRendererID());
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
}