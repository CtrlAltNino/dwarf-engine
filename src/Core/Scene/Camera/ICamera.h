#pragma once

#include "Core/Scene/Components/SceneComponents.h"
#include "Utilities/ISerializable.h"

namespace Dwarf
{
  /**
   * @brief Struct containing all properties of a camera
   *
   */
  struct CameraProperties
  {
    /// @brief The transform of the camera, representing its position and
    /// rotation.
    TransformComponent Transform;
    /// @brief The camera's field of view expressed in degrees.
    float Fov = 90.0F;

    /// @brief The near plane distance.
    float NearPlane = 0.1F;

    /// @brief The far plane distance.
    float FarPlane = 1000.0F;

    /// @brief The aspect ratio of the camera.
    float AspectRatio = 16.0F / 9.0F;

    float Sensitivity = 0.15F;

    float MovementSpeed = 4.0F;
  };

  /**
   * @brief Class representing a camera
   *
   */
  class ICamera : public ISerializable
  {
  public:
    virtual ~ICamera() = default;

    /// @brief Returns the view matrix of the camera.
    /// @return 4x4 view matrix.
    [[nodiscard]] virtual auto
    GetViewMatrix() const -> glm::mat4x4 = 0;

    /// @brief Returns the projection matrix of the camera.
    /// @return 4x4 projection matrix.
    [[nodiscard]] virtual auto
    GetProjectionMatrix() const -> glm::mat4x4 = 0;

    /**
     * @brief Gets the properties of the camera instance
     *
     * @return Reference to the camera properties
     */
    virtual auto
    GetProperties() -> CameraProperties& = 0;

    /**
     * @brief Processes camera movement and rotation
     *
     * @param deltaTime Current delta time
     */
    virtual void
    OnUpdate(double deltaTime) = 0;

    /**
     * @brief Projects a 2 dimensional screen coordinate on the near plane of
     * the camera in world space
     *
     * @param screenPosition 2d pixel coordinate
     * @param viewport Viewport dimensions
     * @return Projected 3d coordinate
     */
    [[nodiscard]] virtual auto
    ScreenToWorld(glm::vec2 const& screenPosition,
                  glm::vec2 const& viewport) const -> glm::vec3 = 0;

    auto
    Serialize() -> nlohmann::json override = 0;
  };
}