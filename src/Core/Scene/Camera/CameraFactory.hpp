#pragma once

#include "ICameraFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class CameraFactory : public ICameraFactory
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;

  public:
    CameraFactory(std::shared_ptr<IDwarfLogger> logger);
    ~CameraFactory() override;

    /**
     * @brief Creates a camera instance
     *
     * @return Unique pointer to the created camera instance
     */
    [[nodiscard]] auto
    Create() const -> std::unique_ptr<ICamera> override;

    /**
     * @brief Creates a camera instance with the specified properties
     *
     * @param properties Properties for the camera
     * @return Unique pointer to the created camera instance
     */
    [[nodiscard]] auto
    Create(CameraProperties properties) const
      -> std::unique_ptr<ICamera> override;

    /**
     * @brief Creates a camera from serialized camera data
     *
     * @param serializedCameraData JSON structure containing camera data
     * @return Unique pointer to the created camera instance
     */
    [[nodiscard]] auto
    Create(const nlohmann::json& serializedCameraData) const
      -> std::unique_ptr<ICamera> override;
  };
} // namespace Dwarf