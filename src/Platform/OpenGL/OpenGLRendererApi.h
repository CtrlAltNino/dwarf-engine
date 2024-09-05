#pragma once
#include "Core/Asset/Database/IAssetDatabase.h"
#include "pch.h"

#include "Core/Rendering/RendererApi/IRendererApi.h"

namespace Dwarf
{
  class OpenGLRendererApi : public IRendererApi
  {
  public:
    OpenGLRendererApi();
    ~OpenGLRendererApi() override;

    void
    Init() override;
    void
    SetViewport(uint32_t x,
                uint32_t y,
                uint32_t width,
                uint32_t height) override;

    void
    SetClearColor(const glm::vec4& color) override;
    void
    Clear(unsigned int value) override;
    void
    Clear() override;

    void
    RenderIndexed(std::shared_ptr<IMesh>     mesh,
                  std::shared_ptr<IMaterial> material,
                  glm::mat4                  modelMatrix,
                  glm::mat4                  viewMatrix,
                  glm::mat4                  projectionMatrix) override;
    void
    ApplyComputeShader(std::shared_ptr<IComputeShader> computeShader,
                       std::shared_ptr<IFramebuffer>   fb,
                       uint32_t                        sourceAttachment,
                       uint32_t destinationAttachment) override;

    void
    Blit(std::shared_ptr<IFramebuffer> source,
         std::shared_ptr<IFramebuffer> destination,
         uint32_t                      sourceAttachment,
         uint32_t                      destinationAttachment,
         uint32_t                      width,
         uint32_t                      height) override;
  };
}