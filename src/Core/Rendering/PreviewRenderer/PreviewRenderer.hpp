#pragma once

#include "Core/Rendering/Framebuffer/IFramebuffer.hpp"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.hpp"
#include "Core/Rendering/RendererApi/IRendererApi.hpp"
#include "Core/Rendering/RendererApi/IRendererApiFactory.hpp"
#include "Core/Scene/Camera/ICamera.hpp"
#include "Editor/Stats/IEditorStats.hpp"
#include <imgui.h>

namespace Dwarf
{
  /**
   * @brief Struct that contains the properties of the preview renderer
   *
   */
  struct PreviewRendererProperties
  {
    glm::vec3 ModelRotation = { 0, 0, 0 };
    glm::vec3 ModelRotationTarget = { 0, 0, 0 };
    glm::quat ModelRotationQuat = glm::quat({ 0, 0, 0 });
    float     RotationSpeed = 0.1F;
    float     ScrollSpeed = 0.3F;
    float     MaxDistance = 0.0F;
    float     Distance = 1.0F;
  };

  /**
   * @brief Base class for preview renderers. It's task is primitively rendering
   * to a single framebuffer and rotate the object.
   *
   */
  class PreviewRenderer
  {
  protected:
    PreviewRendererProperties            mProperties;
    std::shared_ptr<IFramebufferFactory> mFramebufferFactory;
    std::shared_ptr<IFramebuffer>        mRenderFramebuffer;
    std::shared_ptr<IFramebuffer>        mPreviewFramebuffer;
    std::shared_ptr<ICamera>             mCamera;
    std::shared_ptr<IRendererApi>        mRendererApi;
    std::shared_ptr<IEditorStats>        mEditorStats;

    /**
     * @brief Helper easing function
     *
     * @param t Value to apply the easing function
     * @return Value with applied easing
     */
    [[nodiscard]] auto
    EaseInOutQuad(float t) const -> float;

    /**
     * @brief Interpolates between two vectors based on a scalar
     *
     * @param currentVector First vector
     * @param targetVector Second vector
     * @param speed Scalar to interpolate with (Must be between 0.0 and 1.0)
     * @return Interpolated vector
     */
    [[nodiscard]] auto
    InterpolateVectors(const glm::vec3& currentVector,
                       const glm::vec3& targetVector,
                       float            speed) const -> glm::vec3;

  public:
    PreviewRenderer(
      std::shared_ptr<IFramebufferFactory>        framebufferFactory,
      std::shared_ptr<ICamera>                    camera,
      const std::shared_ptr<IRendererApiFactory>& rendererApiFactory,
      std::shared_ptr<IEditorStats>               editorStats);
    /**
     * @brief Resizes the framebuffer of the preview framebuffer
     *
     * @param size Size for the framebuffer
     */
    void
    Resize(glm::ivec2 size);

    /**
     * @brief Updates the rotation based on the provided delta mouse position
     *
     * @param deltaMousePos Difference of the mouse position to the last frame
     */
    void
    UpdateRotation(glm::vec2 deltaMousePos);

    /**
     * @brief Returns the scroll distance
     *
     * @return Scroll distance
     */
    [[nodiscard]] auto
    GetScrollDistance() const -> float;

    /**
     * @brief Sets the scrolls distance
     *
     * @param distance Scroll distance
     */
    void
    SetScrollDistance(float distance);

    /**
     * @brief Retrieves the ImGui texture id of the framebuffer
     *
     * @return ImGui texture id
     */
    [[nodiscard]] auto
    GetTextureId() const -> ImTextureID;
  };
}