#pragma once

#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/RendererApi.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Camera.h"

namespace Dwarf
{
  class DeferredRenderer : public Renderer
  {
  private:
    std::shared_ptr<RendererApi> rendererApi;

  public:
    DeferredRenderer();
    ~DeferredRenderer();
    virtual void
    RenderScene(std::shared_ptr<Scene>  scene,
                std::shared_ptr<Camera> camera,
                glm::ivec2              viewportSize,
                bool                    renderGrid) override;
    virtual std::shared_ptr<Framebuffer>
    CreateFramebuffer(glm::ivec2 resolution) override;
  };
}