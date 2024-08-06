#pragma once
#include "ICamera.h"
#include "Input/IInputManager.h"
#include "pch.h"
#include <memory>

namespace Dwarf
{

  /// @brief Class representing a camera.
  class Camera : public ICamera
  {
  private:
    std::shared_ptr<IInputManager> m_InputManager;
    CameraProperties               m_Properties;

  public:
    // ========== Constructors ==========

    Camera(std::shared_ptr<IInputManager> inputManager,
           CameraProperties               properties = CameraProperties());

    Camera(std::shared_ptr<IInputManager> inputManager, nlohmann::json json);

    // ========== Getters ==========

    /// @brief Returns the field of view.
    /// @return Fov of the camera.
    float
    GetFov() const override;

    /// @brief Returns the near and far plane distances.
    /// @return The near and far plane distances as a 2D vector.
    glm::vec2
    GetRenderPlaneParameters() const override;

    /// @brief Returns the aspect ratio of the camera.
    /// @return Aspect ratio.
    float
    GetAspectRatio() const override;

    /// @brief Returns the view matrix of the camera.
    /// @return 4x4 view matrix.
    glm::mat4x4
    GetViewMatrix() const override;

    /// @brief Returns the projection matrix of the camera.
    /// @return 4x4 projection matrix.
    glm::mat4x4
    GetProjectionMatrix() const override;

    CameraProperties&
    GetProperties() override;

    // ========== Camera Functions ==========
    void
    OnUpdate(double deltaTime) override;

    nlohmann::json
    Serialize() const override;

    glm::vec3
    ScreenToWorld(glm::vec2 const& screenPosition,
                  glm::vec2 const& viewport) const override;
  };
}