#pragma once
#include "ICamera.h"
#include "Input/IInputManager.h"
#include "Logging/IDwarfLogger.h"
#include <memory>

namespace Dwarf
{

  /// @brief Class representing a camera.
  class Camera : public ICamera
  {
  private:
    std::shared_ptr<IDwarfLogger>  mLogger;
    std::shared_ptr<IInputManager> mInputManager;
    CameraProperties               mProperties;

  public:
    // ========== Constructors ==========

    Camera(std::shared_ptr<IDwarfLogger>  logger,
           std::shared_ptr<IInputManager> inputManager,
           CameraProperties               properties = CameraProperties());

    Camera(std::shared_ptr<IDwarfLogger>  logger,
           std::shared_ptr<IInputManager> inputManager,
           const nlohmann::json&          serializedCameraData);

    ~Camera() override;

    /// @brief Returns the view matrix of the camera.
    /// @return 4x4 view matrix.
    [[nodiscard]] auto
    GetViewMatrix() const -> glm::mat4x4 override;

    /// @brief Returns the projection matrix of the camera.
    /// @return 4x4 projection matrix.
    [[nodiscard]] auto
    GetProjectionMatrix() const -> glm::mat4x4 override;

    /**
     * @brief Gets the properties of the camera instance
     *
     * @return Reference to the camera properties
     */
    auto
    GetProperties() -> CameraProperties& override;

    /**
     * @brief Processes camera movement and rotation
     *
     * @param deltaTime Current delta time
     */
    void
    OnUpdate(double deltaTime) override;

    /**
     * @brief Projects a 2 dimensional screen coordinate on the near plane of
     * the camera in world space
     *
     * @param screenPosition 2d pixel coordinate
     * @param viewport Viewport dimensions
     * @return Projected 3d coordinate
     */
    [[nodiscard]] auto
    ScreenToWorld(glm::vec2 const& screenPosition,
                  glm::vec2 const& viewport) const -> glm::vec3 override;

    auto
    Serialize() -> nlohmann::json override;
  };
}