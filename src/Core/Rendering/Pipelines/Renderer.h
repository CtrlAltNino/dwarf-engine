#pragma once

#include "pch.h"

#include "Core/Rendering/Material/Material.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Camera/Camera.h"
#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Database/AssetReference.h"
#include "Core/Rendering/RendererApi/RendererApi.h"

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