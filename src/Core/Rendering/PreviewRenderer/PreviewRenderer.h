#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Core/Rendering/RendererApi/IRendererApiFactory.h"
#include "Core/Scene/Camera/ICamera.h"
#include "Editor/Stats/IEditorStats.h"
#include <imgui.h>
namespace Dwarf
{
  struct PreviewRendererProperties
  {
    glm::vec3 ModelRotation = { 0, 0, 0 };
    glm::vec3 ModelRotationTarget = { 0, 0, 0 };
    glm::quat ModelRotationQuat = glm::quat({ 0, 0, 0 });
    float     RotationSpeed = 0.1f;
    float     ScrollSpeed = 0.3f;
    float     MaxDistance = 0.0f;
    float     Distance = 1.0f;
  };

  class PreviewRenderer
  {
  protected:
    std::shared_ptr<IFramebufferFactory> mFramebufferFactory;
    std::shared_ptr<IFramebuffer>        mRenderFramebuffer;
    std::shared_ptr<IFramebuffer>        mPreviewFramebuffer;
    std::shared_ptr<ICamera>             mCamera;
    PreviewRendererProperties            mProperties;
    std::shared_ptr<IRendererApi>        mRendererApi;
    std::shared_ptr<IEditorStats>        mEditorStats;

  public:
    PreviewRenderer(std::shared_ptr<IFramebufferFactory> framebufferFactory,
                    std::shared_ptr<ICamera>             camera,
                    std::shared_ptr<IRendererApiFactory> rendererApiFactory,
                    std::shared_ptr<IEditorStats>        editorStats);

    void
    Resize(glm::ivec2 size);

    void
    UpdateRotation(glm::vec2 deltaMousePos);

    float
    GetScrollDistance() const;

    void
    SetScrollDistance(float distance);

    ImTextureID
    GetTextureId() const;

    float
    EaseInOutQuad(float t) const;

    glm::vec3
    InterpolateVectors(const glm::vec3& currentVector,
                       const glm::vec3& targetVector,
                       float            speed) const;
  };
}