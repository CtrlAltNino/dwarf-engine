#include <glad/glad.h>
#include "OpenGLFramebuffer.h"

#include "Platform/OpenGL/OpenGLRendererApi.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Platform/OpenGL/OpenGLComputeShader.h"
#include "Core/Asset/AssetComponents.h"

#include <Core/Rendering/Shader Parameters/Tex2DShaderParameter.h>
#include <Core/Rendering/Shader Parameters/BooleanShaderParameter.h>
#include <Core/Rendering/Shader Parameters/IntegerShaderParameter.h>
#include <Core/Rendering/Shader Parameters/UnsignedIntegerShaderParameter.h>
#include <Core/Rendering/Shader Parameters/FloatShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Vec4ShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Vec2ShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Vec3ShaderParameter.h>
#include "Platform/OpenGL/OpenGLMesh.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Dwarf
{
  OpenGLRendererApi::OpenGLRendererApi() = default;
  OpenGLRendererApi::~OpenGLRendererApi() = default;

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
  OpenGLRendererApi::RenderIndexed(std::shared_ptr<Mesh>     mesh,
                                   std::shared_ptr<Material> material,
                                   glm::mat4                 modelMatrix,
                                   glm::mat4                 viewMatrix,
                                   glm::mat4                 projectionMatrix)
  {
    std::shared_ptr<OpenGLMesh> oglMesh =
      std::dynamic_pointer_cast<OpenGLMesh>(mesh);
    std::shared_ptr<OpenGLShader> shader =
      std::dynamic_pointer_cast<OpenGLShader>(material->GetShader());
    char textureInputCounter = 0;

    glUseProgram(shader->GetID());

    if (material->IsTransparent())
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // TODO: Move this to OpenGLShader.cpp
    for (auto const& [key, val] : material->m_Parameters)
    {
      if (val)
      {
        switch ((*val).GetType())
        {
          using enum ShaderParameterType;
          case BOOLEAN:
            glUniform1f(
              glGetUniformLocation(shader->GetID(), key.c_str()),
              (float)std::dynamic_pointer_cast<BooleanShaderParameter>(val)
                ->m_Value);
            break;
          case INTEGER:
            glUniform1i(
              glGetUniformLocation(shader->GetID(), key.c_str()),
              std::dynamic_pointer_cast<IntegerShaderParameter>(val)->m_Value);
            break;
          case UNSIGNED_INTEGER:
            glUniform1ui(
              glGetUniformLocation(shader->GetID(), key.c_str()),
              std::dynamic_pointer_cast<UnsignedIntegerShaderParameter>(val)
                ->m_Value);
            break;
          case FLOAT:
            glUniform1f(
              glGetUniformLocation(shader->GetID(), key.c_str()),
              std::dynamic_pointer_cast<FloatShaderParameter>(val)->m_Value);
            break;
          case TEX2D:
            {
              std::shared_ptr<UID> parameter =
                std::dynamic_pointer_cast<Tex2DShaderParameter>(val)->m_Value;
              if (parameter)
              {
                glActiveTexture(GL_TEXTURE0 + textureInputCounter);
                glBindTexture(GL_TEXTURE_2D,
                              AssetDatabase::Retrieve<TextureAsset>(parameter)
                                ->GetAsset()
                                ->m_Texture->GetTextureID());

                GLuint uniformID =
                  glGetUniformLocation(shader->GetID(), key.c_str());
                glUniform1i(uniformID, textureInputCounter);
              }

              textureInputCounter++;
            }
            break;
          case VEC2:
            {
              glm::vec2 parameter =
                std::dynamic_pointer_cast<Vec2ShaderParameter>(val)->m_Value;
              glUniform2f(glGetUniformLocation(shader->GetID(), key.c_str()),
                          parameter.x,
                          parameter.y);
            }
            break;
          case VEC3:
            {
              glm::vec3 parameter =
                std::dynamic_pointer_cast<Vec3ShaderParameter>(val)->m_Value;
              glUniform3f(glGetUniformLocation(shader->GetID(), key.c_str()),
                          parameter.x,
                          parameter.y,
                          parameter.z);
            }
            break;
          case VEC4:
            {
              glm::vec4 parameter =
                std::dynamic_pointer_cast<Vec4ShaderParameter>(val)->m_Value;
              glUniform4f(glGetUniformLocation(shader->GetID(), key.c_str()),
                          parameter.x,
                          parameter.y,
                          parameter.z,
                          parameter.w);
            }
            break;
        }
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
    std::shared_ptr<ComputeShader> computeShader,
    std::shared_ptr<Framebuffer>   fb,
    uint32_t                       sourceAttachment,
    uint32_t                       destinationAttachment)
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
  OpenGLRendererApi::Blit(std::shared_ptr<Framebuffer> source,
                          std::shared_ptr<Framebuffer> destination,
                          uint32_t                     sourceAttachment,
                          uint32_t                     destinationAttachment,
                          uint32_t                     width,
                          uint32_t                     height)
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