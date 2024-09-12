#pragma once

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
    RenderEntity(Entity&   entity,
                 glm::mat4 viewMatrix,
                 glm::mat4 projectionMatrix) = 0;

    virtual void
    RenderScene(IScene&    scene,
                ICamera&   camera,
                glm::ivec2 viewportSize,
                bool       renderGrid) = 0;
    // virtual void
    // RenderIds(std::shared_ptr<IScene>  scene,
    //           std::shared_ptr<ICamera> camera,
    //           glm::ivec2               viewportSize) = 0;

    // virtual void
    // RenderModelPreview(std::shared_ptr<AssetReference<ModelAsset>>
    // modelAsset,
    //                    std::shared_ptr<ICamera>                    camera,
    //                    glm::ivec2 viewportSize, glm::quat rotation) = 0;
    // virtual void
    // RenderMaterialPreview(
    //   std::shared_ptr<AssetReference<MaterialAsset>> materialAsset,
    //   std::shared_ptr<ICamera>                       camera,
    //   glm::ivec2                                     viewportSize,
    //   glm::quat                                      rotation) = 0;
  };
}