#include "pch.h"

#include "Camera.h"
#include "CameraFactory.h"

namespace Dwarf
{
  CameraFactory::CameraFactory(std::shared_ptr<IDwarfLogger>  logger,
                               std::shared_ptr<IInputManager> inputManager)
    : mLogger(std::move(logger))
    , mInputManager(std::move(inputManager))
  {
    mLogger->LogDebug(Log("CameraFactory created.", "CameraFactory"));
  }

  CameraFactory::~CameraFactory()
  {
    mLogger->LogDebug(Log("CameraFactory destroyed.", "CameraFactory"));
  }

  auto
  CameraFactory::Create() const -> std::unique_ptr<ICamera>
  {
    return std::make_unique<Camera>(mLogger, mInputManager);
  }

  auto
  CameraFactory::Create(glm::vec3 position, glm::vec3 rotation) const
    -> std::unique_ptr<ICamera>
  {
    return std::make_unique<Camera>(
      mLogger, mInputManager, CameraProperties({ position, rotation }));
  }

  auto
  CameraFactory::Create(glm::vec3 position,
                        glm::vec3 rotation,
                        float     fov,
                        float     nearPlane,
                        float     farPlane,
                        float     aspectRatio) const -> std::unique_ptr<ICamera>
  {
    return std::make_unique<Camera>(
      mLogger,
      nullptr,
      CameraProperties(
        { position, rotation }, fov, nearPlane, farPlane, aspectRatio));
  }

  auto
  CameraFactory::Create(const nlohmann::json& serializedCameraData) const
    -> std::unique_ptr<ICamera>
  {
    return std::make_unique<Camera>(
      mLogger, mInputManager, serializedCameraData);
  }
}