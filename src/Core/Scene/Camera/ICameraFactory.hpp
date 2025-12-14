#pragma once

#include "Core/Scene/Camera/ICamera.hpp"

namespace Dwarf
{
  /**
   * @brief Factory class that creates Camera instances
   *
   */
  class ICameraFactory
  {
  public:
    virtual ~ICameraFactory() = default;

    /**
     * @brief Creates a camera instance
     *
     * @return Unique pointer to the created camera instance
     */
    [[nodiscard]] virtual auto
    Create() const -> std::unique_ptr<ICamera> = 0;

    /**
     * @brief Creates a camera instance with the specified properties
     *
     * @param properties Properties for the camera
     * @return Unique pointer to the created camera instance
     */
    [[nodiscard]] virtual auto
    Create(CameraProperties properties) const -> std::unique_ptr<ICamera> = 0;

    /**
     * @brief Creates a camera from serialized camera data
     *
     * @param serializedCameraData JSON structure containing camera data
     * @return Unique pointer to the created camera instance
     */
    [[nodiscard]] virtual auto
    Create(const nlohmann::json& serializedCameraData) const
      -> std::unique_ptr<ICamera> = 0;
  };
}