

#include "Core/Rendering/Deferred/DeferredRenderer.h"

namespace Dwarf
{
  DeferredRenderer::DeferredRenderer() {}
  DeferredRenderer::~DeferredRenderer() {}

  void
  DeferredRenderer::RenderScene(std::shared_ptr<Scene>  scene,
                                std::shared_ptr<Camera> camera,
                                glm::ivec2              viewportSize,
                                bool                    renderGrid)
  {
  }

  std::shared_ptr<Framebuffer>
  DeferredRenderer::CreateFramebuffer(glm::ivec2 resolution)
  {
    return nullptr;
  }
}