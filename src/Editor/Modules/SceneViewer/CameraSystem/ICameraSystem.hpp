#pragma once

#include "Core/Scene/Components/CameraComponentHandle.hpp"

namespace Dwarf
{
  /**
   * @brief Runtime properties of the camera system
   *
   */
  struct CameraSystemProperties
  {
    float Sensitivity = 0.15F;
    float MovementSpeed = 4.0F;
  };

  /**
   * @brief Interface class defining a camera system
   *
   */
  class ICameraSystem : public ISerializable
  {
  public:
    virtual ~ICameraSystem() = default;

    /**
     * @brief Slots in a camera instance to use
     *
     * @param handle A safe handle to the entity holding a CameraComponent
     */
    virtual void
    SetCamera(CameraComponentHandle handle) = 0;

    /**
     * @brief Returns the handle to the currently used camera component
     *
     * @return CameraComponentHandle The handle to the camera component
     */
    virtual auto
    GetCamera() -> CameraComponentHandle = 0;

    /**
     * @brief Checks if the camera system currently contains a camera
     *
     * @return true Camera present
     * @return false No camera present
     */
    [[nodiscard]] virtual auto
    HasCamera() const -> bool = 0;

    /**
     * @brief Returns the view matrix of the camera system
     *
     * @return glm::mat4x4 View matrix
     */
    [[nodiscard]] virtual auto
    GetViewMatrix() const -> glm::mat4x4 = 0;

    /**
     * @brief Returns the projection matrix of the camera system
     *
     * @param aspectRatio Aspect ratio to use for the projection matrix
     * @return glm::mat4x4 Calculated projection matrix
     */
    [[nodiscard]] virtual auto
    GetProjectionMatrix(float aspectRatio) const -> glm::mat4x4 = 0;

    /**
     * @brief Returns the current position of the camera
     *
     * @return glm::vec3 Position of the camera
     */
    [[nodiscard]] virtual auto
    GetPosition() const -> glm::vec3 = 0;

    /**
     * @brief Returns the distance of the near plane
     *
     * @return float Near plane value
     */
    [[nodiscard]] virtual auto
    GetNearplane() const -> float = 0;

    /**
     * @brief Returns the distance of the far plane
     *
     * @return float Far plane value
     */
    [[nodiscard]] virtual auto
    GetFarplane() const -> float = 0;

    /**
     * @brief Returns the field of view
     *
     * @return float Field of view value
     */
    [[nodiscard]] virtual auto
    GetFov() const -> float = 0;

    /**
     * @brief Sets the near plane value
     *
     * @param nearplane Value to use
     */
    virtual void
    SetNearplane(float nearplane) = 0;

    /**
     * @brief Sets the far plane value
     *
     * @param farplane Value to use
     */
    virtual void
    SetFarplane(float farplane) = 0;

    /**
     * @brief Sets the field of view value
     *
     * @param fov Field of view value
     */
    virtual void
    SetFov(float fov) = 0;

    /**
     * @brief The camera system logic that needs to run every frame
     *
     * @param deltaTime Time since last update
     */
    virtual void
    OnUpdate(double deltaTime) = 0;

    /**
     * @brief Transforms a 2D screen position to a 3D world position (On the
     * near plane)
     *
     * @param screenPosition 2D pixel position
     * @param viewport Width and height on the screen (In pixels)
     * @return glm::vec3
     */
    [[nodiscard]] virtual auto
    ScreenToWorld(glm::vec2 const& screenPosition,
                  glm::vec2 const& viewport) const -> glm::vec3 = 0;

    auto
    Serialize() -> nlohmann::json override = 0;
  };
}