#pragma once

#include "Core/Scene/Components/SceneComponents.h"
#include "Utilities/ISerializable.h"
namespace Dwarf
{
  struct CameraProperties
  {
    /// @brief The transform of the camera, representing its position and
    /// rotation.
    TransformComponent transform;
    /// @brief The camera's field of view expressed in degrees.
    float m_Fov = 90.0f;

    /// @brief The near plane distance.
    float m_NearPlane = 0.1f;

    /// @brief The far plane distance.
    float m_FarPlane = 1000.0f;

    /// @brief The aspect ratio of the camera.
    float m_AspectRatio = 16.0f / 9.0f;

    float m_sensitivity = 0.15f;

    float m_MovementSpeed = 4.0f;
  };
  class ICamera : public ISerializable
  {
  public:
    /// @brief Returns the field of view.
    /// @return Fov of the camera.
    virtual float
    GetFov() const;

    /// @brief Returns the near and far plane distances.
    /// @return The near and far plane distances as a 2D vector.
    virtual glm::vec2
    GetRenderPlaneParameters() const;

    /// @brief Returns the aspect ratio of the camera.
    /// @return Aspect ratio.
    virtual float
    GetAspectRatio() const;

    /// @brief Returns the view matrix of the camera.
    /// @return 4x4 view matrix.
    virtual glm::mat4x4
    GetViewMatrix() const;

    /// @brief Returns the projection matrix of the camera.
    /// @return 4x4 projection matrix.
    virtual glm::mat4x4
    GetProjectionMatrix() const;

    virtual CameraProperties&
    GetProperties();

    // ========== Camera Functions ==========
    virtual void
    OnUpdate(double deltaTime);

    virtual glm::vec3
    ScreenToWorld(glm::vec2 const& screenPosition,
                  glm::vec2 const& viewport) const;
  };
}