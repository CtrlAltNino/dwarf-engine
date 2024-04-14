#pragma once

#include <glm/glm.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include "Core/Rendering/Material.h"
#include "Core/Rendering/Mesh.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Camera.h"
#include "Core/Rendering/Framebuffer.h"
#include "Core/Asset/AssetComponents.h"
#include "Core/Asset/AssetReference.h"
#include "Core/Rendering/RendererApi.h"

namespace Dwarf {
  class Renderer
  {
  public:
    enum class RendererType
    {
      Forward,
      Deferred
    };
    virtual ~Renderer() = default;
    static GraphicsApi GetAPI();
    static RendererType GetType();
    static void Create(GraphicsApi api, Renderer::RendererType type);
    static Ref<Renderer> Get() { return s_Renderer; }
    // TODO: Maybe pass framebuffer?
    virtual void RenderEntity(Entity& entity,
                              glm::mat4 viewMatrix,
                              glm::mat4 projectionMatrix,
                              Ref<Material> overrideMaterial = nullptr) = 0;
    virtual void RenderScene(Ref<Scene> scene,
                             Ref<Camera> camera,
                             glm::ivec2 viewportSize,
                             bool renderGrid) = 0;
    virtual void RenderIds(Ref<Scene> scene,
                           Ref<Camera> camera,
                           glm::ivec2 viewportSize) = 0;

    virtual void RenderModelPreview(Ref<AssetReference<ModelAsset>> modelAsset,
                                    Ref<Camera> camera,
                                    glm::ivec2 viewportSize,
                                    glm::quat rotation) = 0;
    virtual void RenderMaterialPreview(
      Ref<AssetReference<MaterialAsset>> materialAsset,
      Ref<Camera> camera,
      glm::ivec2 viewportSize,
      glm::quat rotation) = 0;

    virtual Ref<Framebuffer> CreateFramebuffer(glm::ivec2 resolution) = 0;
    virtual Ref<Framebuffer> CreateIDFramebuffer(glm::ivec2 resolution) = 0;

    virtual Ref<RendererApi> GetRendererApi() = 0;

  private:
    static GraphicsApi s_Api;
    static RendererType s_RendererType;
    static Ref<Renderer> s_Renderer;
  };
}