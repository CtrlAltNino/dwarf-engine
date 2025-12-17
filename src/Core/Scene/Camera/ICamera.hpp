#pragma once

#include "Utilities/ISerializable.hpp"

namespace Dwarf
{
  /**
   * @brief Struct containing all properties of a camera
   *
   */
  struct CameraProperties
  {
    /// @brief The camera's field of view expressed in degrees.
    float Fov = 90.0F;

    /// @brief The near plane distance.
    float NearPlane = 0.1F;

    /// @brief The far plane distance.
    float FarPlane = 1000.0F;
  };

  /**
   * @brief Class representing a camera
   *
   */
  class ICamera : public ISerializable
  {
  public:
    virtual ~ICamera() = default;

    /// @brief Returns the projection matrix of the camera.
    /// @return 4x4 projection matrix.
    [[nodiscard]] virtual auto
    GetProjectionMatrix(float aspectRatio) const -> glm::mat4x4 = 0;

    /**
     * @brief Gets the properties of the camera instance
     *
     * @return Reference to the camera properties
     */
    virtual auto
    GetProperties() -> CameraProperties& = 0;

    auto
    Serialize() -> nlohmann::json override = 0;
  };
}