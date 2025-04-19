#pragma once

#include "ICameraFactory.hpp"
#include "Input/IInputManager.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class CameraFactory : public ICameraFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>  mLogger;
    std::shared_ptr<IInputManager> mInputManager;

  public:
    CameraFactory(std::shared_ptr<IDwarfLogger>  logger,
                  std::shared_ptr<IInputManager> inputManager);
    ~CameraFactory() override;

    /**
     * @brief Creates a camera instance
     *
     * @return Unique pointer to the created camera instance
     */
    [[nodiscard]] auto
    Create() const -> std::unique_ptr<ICamera> override;

    /**
     * @brief Creates a camera with a specified position and rotation
     *
     * @param position Position for the camera
     * @param rotation Rotation for the camera
     * @return Unique pointer to the created camera instance
     */
    [[nodiscard]] auto
    Create(glm::vec3 position,
           glm::vec3 rotation) const -> std::unique_ptr<ICamera> override;

    /**
     * @brief Creates a camera instance with the specified properties
     *
     * @param position Position for the camera
     * @param rotation Rotation for the camera
     * @param fov Field of view for the camera
     * @param nearPlane Near plane for the camera
     * @param farPlane Far plane for the camera
     * @param aspectRatio Aspect ratio for the camera
     * @return Unique pointer to the created camera instance
     */
    [[nodiscard]] auto
    Create(glm::vec3 position,
           glm::vec3 rotation,
           float     fov,
           float     nearPlane,
           float     farPlane,
           float     aspectRatio) const -> std::unique_ptr<ICamera> override;

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