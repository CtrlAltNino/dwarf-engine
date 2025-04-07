#pragma once

#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Scene/Camera/ICamera.h"
#include "Core/Scene/IScene.h"
#include "Utilities/ISerializable.h"

namespace Dwarf
{
  enum TonemapType
  {
    Reinhard,
    Agx,
    Aces
  };

  struct GridSettings : public ISerializable
  {
    bool  RenderGrid = true;
    float GridOpacity = 1.0F;
    float GridYOffset = 0.0F;

    GridSettings() = default;

    GridSettings(const nlohmann::json& serializedGridSettings)
      : RenderGrid(serializedGridSettings["renderGrid"].get<bool>())
      , GridOpacity(serializedGridSettings["opacity"].get<float>())
      , GridYOffset(serializedGridSettings["offset"].get<float>())
    {
    }

    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json serializedGridSettings;

      serializedGridSettings["renderGrid"] = RenderGrid;
      serializedGridSettings["opacity"] = GridOpacity;
      serializedGridSettings["offset"] = GridYOffset;

      return serializedGridSettings;
    }
  };
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
    RenderScene(ICamera& camera, GridSettings gridSettings) = 0;

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
    RenderIds(IScene& scene, ICamera& camera) = 0;

    /**
     * @brief Gets the resolution
     *
     * @return glm::ivec2 resolution
     */
    virtual auto
    GetResolution() -> glm::ivec2 = 0;

    /**
     * @brief Sets the resolution
     *
     * @param resolution resolution in pixels
     */
    virtual void
    SetResolution(glm::ivec2 resolution) = 0;

    /**
     * @brief Sets the MSAA sample amount
     *
     * @param samples Amount of samples per pixels
     */
    virtual void
    SetMsaaSamples(int32_t samples) = 0;

    /**
     * @brief Gets the MSAA sample amount
     *
     * @return Amount of samples
     */
    virtual auto
    GetMsaaSamples() -> int32_t = 0;

    /**
     * @brief Gets the maximum amount of MSAA samples supported on the device
     *
     * @return Maximum amount of samples
     */
    virtual auto
    GetMaxMsaaSamples() -> int32_t = 0;

    /**
     * @brief Reads the id from the id framebuffer at a given position
     *
     * @param position 2d pixel position
     * @return Stored ID
     */
    virtual auto
    ReadPixelId(glm::ivec2 position) -> uint32_t = 0;

    /**
     * @brief Gets the Texture ID for the presentation buffer
     *
     * @return Id to the buffer
     */
    virtual auto
    GetPresentationBufferId() -> uintptr_t = 0;

    /**
     * @brief Sets the exposure value to apply before tone mapping
     *
     * @param exposure Exposure value
     */
    virtual void
    SetExposure(float exposure) = 0;

    /**
     * @brief Gets the amount of current draw calls
     *
     * @return Draw call count
     */
    [[nodiscard]] virtual auto
    GetDrawCallCount() const -> uint32_t = 0;

    /**
     * @brief Gets the total amount of vertices over all draw calls
     *
     * @return Vertex count
     */
    [[nodiscard]] virtual auto
    GetVertexCount() const -> uint32_t = 0;

    /**
     * @brief Gets the total amount of triangles over all draw calls
     *
     * @return Triangle count
     */
    [[nodiscard]] virtual auto
    GetTriangleCount() const -> uint32_t = 0;

    /**
     * @brief Sets the used tonemap
     *
     * @param type Reinhard, Agx or Aces
     */
    virtual void
    SetTonemapType(TonemapType type) = 0;
  };
}