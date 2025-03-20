#include "CameraFactory.h"

#include "Camera.h"

namespace Dwarf
{
  CameraFactory::CameraFactory(std::shared_ptr<IInputManager> inputManager)
    : mInputManager(inputManager)
  {
  }

  std::shared_ptr<ICamera>
  CameraFactory::Create()
  {
    return std::make_shared<Camera>(mInputManager);
  }

  std::shared_ptr<ICamera>
  CameraFactory::Create(glm::vec3 position, glm::vec3 rotation)
  {
    return std::make_shared<Camera>(mInputManager,
                                    CameraProperties({ position, rotation }));
  }

  std::shared_ptr<ICamera>
  CameraFactory::Create(glm::vec3 position,
                        glm::vec3 rotation,
                        float     fov,
                        float     nearPlane,
                        float     farPlane,
                        float     aspectRatio)
  {
    return std::make_shared<Camera>(
      nullptr,
      CameraProperties(
        { position, rotation }, fov, nearPlane, farPlane, aspectRatio));
  }

  std::shared_ptr<ICamera>
  CameraFactory::Create(nlohmann::json json)
  {
    return std::make_shared<Camera>(mInputManager, json);
  }
}