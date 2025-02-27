#pragma once
#include "pch.h"
#include "ICamera.h"
#include "Input/IInputManager.h"
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

    ~Camera() override {};

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
    Serialize() override;

    glm::vec3
    ScreenToWorld(glm::vec2 const& screenPosition,
                  glm::vec2 const& viewport) const override;
  };
}