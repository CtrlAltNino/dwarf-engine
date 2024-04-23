#pragma once

#include "Core/Rendering/Renderer.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Camera.h"

namespace Dwarf
{
  class ForwardRenderer : public Renderer
  {
  private:
    Ref<RendererApi> m_RendererApi;

  public:
    ForwardRenderer();
    virtual ~ForwardRenderer();
    void
    RenderEntity(Entity&       entity,
                 glm::mat4     viewMatrix,
                 glm::mat4     projectionMatrix,
                 Ref<Material> overrideMaterial) override;
    void
    RenderScene(Ref<Scene>  scene,
                Ref<Camera> camera,
                glm::ivec2  viewportSize,
                bool        renderGrid) override;
    void
    RenderIds(Ref<Scene>  scene,
              Ref<Camera> camera,
              glm::ivec2  viewportSize) override;
    void
    RenderModelPreview(Ref<AssetReference<ModelAsset>> modelAsset,
                       Ref<Camera>                     camera,
                       glm::ivec2                      viewportSize,
                       glm::quat                       rotation) override;
    void
    RenderMaterialPreview(Ref<AssetReference<MaterialAsset>> materialAsset,
                          Ref<Camera>                        camera,
                          glm::ivec2                         viewportSize,
                          glm::quat                          rotation) override;
    Ref<Framebuffer>
    CreateFramebuffer(glm::ivec2 resolution) override;
    Ref<Framebuffer>
    CreateIDFramebuffer(glm::ivec2 resolution) override;
    Ref<RendererApi>
    GetRendererApi() override
    {
      return m_RendererApi;
    }
  };
}