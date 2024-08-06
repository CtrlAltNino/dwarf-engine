#pragma once

#include "Core/Rendering/Pipelines/IRenderingPipeline.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Core/Scene/IScene.h"
#include "Core/Scene/Camera/ICamera.h"

namespace Dwarf
{
  class DeferredRenderer : public IRenderingPipeline
  {
  private:
    std::shared_ptr<IRendererApi> m_RendererApi;

  public:
    DeferredRenderer();
    ~DeferredRenderer();
    virtual void
    RenderScene(std::shared_ptr<IScene>  scene,
                std::shared_ptr<ICamera> camera,
                glm::ivec2               viewportSize,
                bool                     renderGrid) override;
  };
}