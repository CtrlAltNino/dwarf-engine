#include "pch.h"
#include "DeferredRenderer.h"

namespace Dwarf
{
  DeferredRenderer::DeferredRenderer() {}
  DeferredRenderer::~DeferredRenderer() {}

  void
  DeferredRenderer::RenderScene(std::shared_ptr<IScene>  scene,
                                std::shared_ptr<ICamera> camera,
                                glm::ivec2               viewportSize,
                                bool                     renderGrid)
  {
  }
}