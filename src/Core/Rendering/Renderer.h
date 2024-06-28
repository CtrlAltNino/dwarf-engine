#pragma once

#include "pch.h"

#include "Core/Rendering/Material/Material.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Camera.h"
#include "Core/Rendering/Framebuffer/Framebuffer.h"
#include "Core/Asset/AssetComponents.h"
#include "Core/Asset/AssetReference.h"
#include "Core/Rendering/RendererApi.h"

namespace Dwarf
{
  class Renderer
  {
  public:
    enum class RendererType
    {
      Forward,
      Deferred
    };
    virtual ~Renderer() = default;
    static GraphicsApi
    GetAPI();
    static RendererType
    GetType();
    static void
    Create(GraphicsApi api, Renderer::RendererType type);
    static std::shared_ptr<Renderer>
    Get()
    {
      return s_Renderer;
    }
    // TODO: Maybe pass framebuffer?
    virtual void
    RenderEntity(Entity&                   entity,
                 glm::mat4                 viewMatrix,
                 glm::mat4                 projectionMatrix,
                 std::shared_ptr<Material> overrideMaterial = nullptr) = 0;
    virtual void
    RenderScene(std::shared_ptr<Scene>  scene,
                std::shared_ptr<Camera> camera,
                glm::ivec2              viewportSize,
                bool                    renderGrid) = 0;
    virtual void
    RenderIds(std::shared_ptr<Scene>  scene,
              std::shared_ptr<Camera> camera,
              glm::ivec2              viewportSize) = 0;

    virtual void
    RenderModelPreview(std::shared_ptr<AssetReference<ModelAsset>> modelAsset,
                       std::shared_ptr<Camera>                     camera,
                       glm::ivec2                                  viewportSize,
                       glm::quat rotation) = 0;
    virtual void
    RenderMaterialPreview(
      std::shared_ptr<AssetReference<MaterialAsset>> materialAsset,
      std::shared_ptr<Camera>                        camera,
      glm::ivec2                                     viewportSize,
      glm::quat                                      rotation) = 0;

    virtual std::shared_ptr<Framebuffer>
    CreateFramebuffer(glm::ivec2 resolution) = 0;
    virtual std::shared_ptr<Framebuffer>
    CreateIDFramebuffer(glm::ivec2 resolution) = 0;

    virtual std::shared_ptr<RendererApi>
    GetRendererApi() = 0;

  private:
    static GraphicsApi               s_Api;
    static RendererType              s_RendererType;
    static std::shared_ptr<Renderer> s_Renderer;
  };
}