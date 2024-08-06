#include <glad/glad.h>
#include "Core/Asset/Database/IAssetDatabase.h"
#include "OpenGLFramebuffer.h"

#include "Platform/OpenGL/OpenGLRendererApi.h"
#include "Platform/OpenGL/OpenGLMesh.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLComputeShader.h"

namespace Dwarf
{
  OpenGLRendererApi::OpenGLRendererApi(
    std::shared_ptr<IAssetDatabase> assetDatabase)
    : m_AssetDatabase(assetDatabase)
  {
  }
  OpenGLRendererApi::~OpenGLRendererApi() = default;

  struct SetShaderParameterVisitor
  {
    GLuint                          m_ShaderID;
    std::string                     m_ParameterName;
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;
    void
    operator()(bool parameter)
    {
      glUniform1f(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter);
    }
    void
    operator()(int parameter)
    {
      glUniform1i(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter);
    }
    void
    operator()(unsigned int parameter)
    {
      glUniform1ui(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                   parameter);
    }
    void
    operator()(float parameter)
    {
      glUniform1f(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter);
    }
    void
    operator()(Texture2DAsset parameter)
    {
      // TODO: This needs to count the number of textures and bind them
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D,
                    m_AssetDatabase->Retrieve<TextureAsset>(parameter)
                      ->GetAsset()
                      ->m_Texture->GetTextureID());
      glUniform1i(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()), 0);
    }
    void
    operator()(glm::vec2 parameter)
    {
      glUniform2f(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter.x,
                  parameter.y);
    }
    void
    operator()(glm::vec3 parameter)
    {
      glUniform3f(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter.x,
                  parameter.y,
                  parameter.z);
    }
    void
    operator()(glm::vec4 parameter)
    {
      glUniform4f(glGetUniformLocation(m_ShaderID, m_ParameterName.c_str()),
                  parameter.x,
                  parameter.y,
                  parameter.z,
                  parameter.w);
    }
  };

  void
  OpenGLRendererApi::Init()
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    SetViewport(0, 0, 512, 512);
  }

  void
  OpenGLRendererApi::SetViewport(uint32_t x,
                                 uint32_t y,
                                 uint32_t width,
                                 uint32_t height)
  {
    glViewport(x, y, width, height);
  }

  void
  OpenGLRendererApi::SetClearColor(const glm::vec4& color)
  {
    glClearColor(color.r, color.g, color.b, color.a);
  }

  void
  OpenGLRendererApi::Clear()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void
  OpenGLRendererApi::Clear(unsigned int value)
  {
    glClearBufferuiv(
      GL_COLOR, 0, &value); // Use glClearBufferuiv for integer types
    glClear(GL_DEPTH_BUFFER_BIT);
  }

  void
  OpenGLRendererApi::RenderIndexed(std::shared_ptr<IMesh>     mesh,
                                   std::shared_ptr<IMaterial> material,
                                   glm::mat4                  modelMatrix,
                                   glm::mat4                  viewMatrix,
                                   glm::mat4                  projectionMatrix)
  {
    std::shared_ptr<OpenGLMesh> oglMesh =
      std::dynamic_pointer_cast<OpenGLMesh>(mesh);
    std::shared_ptr<OpenGLShader> shader =
      std::dynamic_pointer_cast<OpenGLShader>(material->GetShader());
    char textureInputCounter = 0;

    glUseProgram(shader->GetID());

    if (material->GetProperties()->IsTransparent())
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // TODO: Move this to OpenGLShader.cpp
    for (auto const& identifier :
         material->GetParameters()->GetParameterIdentifiers())
    {
      auto shaderParameterValue =
        material->GetParameters()->GetParameter(identifier);
      if (shaderParameterValue.has_value())
      {
        std::visit(SetShaderParameterVisitor{ shader->GetID(), identifier },
                   shaderParameterValue.value());
      }
    }

    GLuint mmID = glGetUniformLocation(shader->GetID(), "modelMatrix");
    GLuint vmID = glGetUniformLocation(shader->GetID(), "viewMatrix");
    GLuint pmID = glGetUniformLocation(shader->GetID(), "projectionMatrix");

    glUniformMatrix4fv(mmID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(vmID, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(pmID, 1, GL_FALSE, &projectionMatrix[0][0]);
    oglMesh->Bind();

    glDrawElements(
      GL_TRIANGLES, oglMesh->GetIndices().size(), GL_UNSIGNED_INT, nullptr);

    oglMesh->Unbind();

    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glUseProgram(0);
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
    glBindImageTexture(0,
                       fb->GetColorAttachment(sourceAttachment)->GetTextureID(),
                       0,
                       GL_FALSE,
                       0,
                       GL_READ_ONLY,
                       GL_RGBA8);
    glBindImageTexture(
      1,
      fb->GetColorAttachment(destinationAttachment)->GetTextureID(),
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