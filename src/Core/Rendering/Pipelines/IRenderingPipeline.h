#pragma once

#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Scene/Camera/ICamera.h"
#include "Core/Scene/IScene.h"

namespace Dwarf
{
  enum class PipelineType
  {
    Forward,
    Deferred
  };
  class IRenderingPipeline
  {
  public:
    virtual ~IRenderingPipeline() = default;

    virtual void
    RenderScene(IScene&    scene,
                ICamera&   camera,
                glm::ivec2 viewportSize,
                bool       renderGrid) = 0;

    virtual FramebufferSpecification
    GetSpecification() = 0;

    virtual void
    RenderIds(IScene& scene, ICamera& camera, glm::ivec2 viewportSize) = 0;
  };
}