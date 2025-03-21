#pragma once

#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Scene/Camera/ICamera.h"
#include "Core/Scene/IScene.h"

namespace Dwarf
{
  class IRenderingPipeline
  {
    /**
     * @brief A class that represents a rendering pipeline
     *
     */
  public:
    virtual ~IRenderingPipeline() = default;

    /**
     * @brief Renders the current draw calls of the scene
     *
     * @param camera Camera to use for rendering
     * @param viewportSize Viewport to render
     * @param renderGrid Should the grid be rendered
     */
    virtual void
    RenderScene(ICamera& camera, glm::ivec2 viewportSize, bool renderGrid) = 0;

    /**
     * @brief Returns the specification for the framebuffer
     *
     * @return Framebuffer specification
     */
    [[nodiscard]] virtual auto
    GetSpecification() const -> FramebufferSpecification = 0;

    /**
     * @brief Renders the scene to the ID buffer for mouse picking
     *
     * @param scene Scene to render
     * @param camera Camera to use for rendering
     * @param viewportSize Viewport to render
     */
    virtual void
    RenderIds(IScene& scene, ICamera& camera, glm::ivec2 viewportSize) = 0;
  };
}