#pragma once
#include "pch.h"

#include "Core/Rendering/RendererApi.h"
#include "Core/Rendering/Material/Material.h"

namespace Dwarf
{
  class OpenGLRendererApi : public RendererApi
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
    RenderIndexed(std::shared_ptr<Mesh>     mesh,
                  std::shared_ptr<Material> material,
                  glm::mat4                 modelMatrix,
                  glm::mat4                 viewMatrix,
                  glm::mat4                 projectionMatrix) override;
    void
    ApplyComputeShader(std::shared_ptr<IComputeShader> computeShader,
                       std::shared_ptr<Framebuffer>    fb,
                       uint32_t                        sourceAttachment,
                       uint32_t destinationAttachment) override;

    void
    Blit(std::shared_ptr<Framebuffer> source,
         std::shared_ptr<Framebuffer> destination,
         uint32_t                     sourceAttachment,
         uint32_t                     destinationAttachment,
         uint32_t                     width,
         uint32_t                     height) override;
  };
}