#pragma once

#include "ICameraFactory.h"
#include "Input/IInputManager.h"

namespace Dwarf
{
  class CameraFactory : public ICameraFactory
  {
  private:
    std::shared_ptr<IInputManager> m_InputManager;

  public:
    CameraFactory(std::shared_ptr<IInputManager> inputManager);
    ~CameraFactory() override = default;
    std::shared_ptr<ICamera>
    Create() override;

    std::shared_ptr<ICamera>
    Create(glm::vec3 position, glm::vec3 rotation) override;

    std::shared_ptr<ICamera>
    Create(glm::vec3 position,
           glm::vec3 rotation,
           float     fov,
           float     nearPlane,
           float     farPlane,
           float     aspectRatio) override;
  };
} // namespace Dwarf