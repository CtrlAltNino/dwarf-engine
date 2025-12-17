#pragma once

#include "ICamera.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <memory>

namespace Dwarf
{

  /// @brief Class representing a camera.
  class Camera : public ICamera
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    CameraProperties              mProperties;

  public:
    // ========== Constructors ==========

    Camera(std::shared_ptr<IDwarfLogger> logger,
           CameraProperties              properties = CameraProperties());

    Camera(std::shared_ptr<IDwarfLogger> logger,
           const nlohmann::json&         serializedCameraData);

    ~Camera() override;

    /// @brief Returns the projection matrix of the camera.
    /// @return 4x4 projection matrix.
    [[nodiscard]] auto
    GetProjectionMatrix(float aspectRatio) const -> glm::mat4x4 override;

    /**
     * @brief Gets the properties of the camera instance
     *
     * @return Reference to the camera properties
     */
    auto
    GetProperties() -> CameraProperties& override;

    auto
    Serialize() -> nlohmann::json override;
  };
}