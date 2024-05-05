#pragma once

#include "Core/Rendering/Renderer.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Camera.h"

namespace Dwarf
{
  class ForwardRenderer : public Renderer
  {
  private:
    std::shared_ptr<RendererApi> m_RendererApi;

  public:
    ForwardRenderer();
    virtual ~ForwardRenderer();
    void
    RenderEntity(Entity&                   entity,
                 glm::mat4                 viewMatrix,
                 glm::mat4                 projectionMatrix,
                 std::shared_ptr<Material> overrideMaterial) override;
    void
    RenderScene(std::shared_ptr<Scene>  scene,
                std::shared_ptr<Camera> camera,
                glm::ivec2              viewportSize,
                bool                    renderGrid) override;
    void
    RenderIds(std::shared_ptr<Scene>  scene,
              std::shared_ptr<Camera> camera,
              glm::ivec2              viewportSize) override;
    void
    RenderModelPreview(std::shared_ptr<AssetReference<ModelAsset>> modelAsset,
                       std::shared_ptr<Camera>                     camera,
                       glm::ivec2                                  viewportSize,
                       glm::quat rotation) override;
    void
    RenderMaterialPreview(
      std::shared_ptr<AssetReference<MaterialAsset>> materialAsset,
      std::shared_ptr<Camera>                        camera,
      glm::ivec2                                     viewportSize,
      glm::quat                                      rotation) override;
    std::shared_ptr<Framebuffer>
    CreateFramebuffer(glm::ivec2 resolution) override;
    std::shared_ptr<Framebuffer>
    CreateIDFramebuffer(glm::ivec2 resolution) override;
    std::shared_ptr<RendererApi>
    GetRendererApi() override
    {
      return m_RendererApi;
    }
  };
}