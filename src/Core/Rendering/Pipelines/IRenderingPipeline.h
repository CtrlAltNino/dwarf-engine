#pragma once

#include "Core/Rendering/DrawCall/IDrawCall.h"
#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Rendering/Material/IMaterial.h"
#include "Core/Scene/Camera/ICamera.h"
#include "Core/Scene/Entity/Entity.h"
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

    // TODO: Maybe pass framebuffer?
    virtual void
    RenderEntity(Entity& entity, ICamera& camera) = 0;

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