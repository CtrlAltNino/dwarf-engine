#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Rendering/Pipelines/IRenderingPipeline.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Camera/Camera.h"

namespace Dwarf
{
  class ForwardRenderer : public IRenderingPipeline
  {
  private:
    std::shared_ptr<IRendererApi>   m_RendererApi;
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;

    void
    Setup(glm::ivec2 viewportSize);

  public:
    ForwardRenderer(std::shared_ptr<IRendererApi> rendererApi);
    ~ForwardRenderer();
    void
    RenderEntity(Entity&                    entity,
                 glm::mat4                  viewMatrix,
                 glm::mat4                  projectionMatrix,
                 std::shared_ptr<IMaterial> overrideMaterial) override;
    void
    RenderScene(std::shared_ptr<IScene>  scene,
                std::shared_ptr<ICamera> camera,
                glm::ivec2               viewportSize,
                bool                     renderGrid) override;
    // void
    // RenderIds(std::shared_ptr<Scene>  scene,
    //           std::shared_ptr<Camera> camera,
    //           glm::ivec2              viewportSize) override;
    // void
    // RenderModelPreview(std::shared_ptr<AssetReference<ModelAsset>>
    // modelAsset,
    //                    std::shared_ptr<Camera>                     camera,
    //                    glm::ivec2 viewportSize, glm::quat rotation) override;
    // void
    // RenderMaterialPreview(
    //   std::shared_ptr<AssetReference<MaterialAsset>> materialAsset,
    //   std::shared_ptr<Camera>                        camera,
    //   glm::ivec2                                     viewportSize,
    //   glm::quat                                      rotation) override;
  };
}