#pragma once

#include "Core/Scene/Components/SceneComponents.h"
#include "Utilities/ISerializable.h"
namespace Dwarf
{
  struct CameraProperties
  {
    /// @brief The transform of the camera, representing its position and
    /// rotation.
    TransformComponent Transform;
    /// @brief The camera's field of view expressed in degrees.
    float Fov = 90.0f;

    /// @brief The near plane distance.
    float NearPlane = 0.1f;

    /// @brief The far plane distance.
    float FarPlane = 1000.0f;

    /// @brief The aspect ratio of the camera.
    float AspectRatio = 16.0f / 9.0f;

    float Sensitivity = 0.15f;

    float MovementSpeed = 4.0f;
  };
  class ICamera : public ISerializable
  {
  public:
    virtual ~ICamera() = default;

    /// @brief Returns the view matrix of the camera.
    /// @return 4x4 view matrix.
    virtual glm::mat4x4
    GetViewMatrix() const = 0;

    /// @brief Returns the projection matrix of the camera.
    /// @return 4x4 projection matrix.
    virtual glm::mat4x4
    GetProjectionMatrix() const = 0;

    virtual CameraProperties&
    GetProperties() = 0;

    // ========== Camera Functions ==========
    virtual void
    OnUpdate(double deltaTime) = 0;

    virtual glm::vec3
    ScreenToWorld(glm::vec2 const& screenPosition,
                  glm::vec2 const& viewport) const = 0;

    virtual nlohmann::json
    Serialize() = 0;
  };
}