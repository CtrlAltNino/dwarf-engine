#pragma once

#include "Core/Scene/Camera/ICamera.h"

namespace Dwarf
{
  class ICameraFactory
  {
  public:
    virtual std::shared_ptr<ICamera>
    Create() = 0;
    virtual std::shared_ptr<ICamera>
    Create(glm::vec3 position, glm::vec3 rotation) = 0;
    virtual std::shared_ptr<ICamera>
    Create(glm::vec3 position,
           glm::vec3 rotation,
           float     fov,
           float     nearPlane,
           float     farPlane,
           float     aspectRatio) = 0;
  };
}