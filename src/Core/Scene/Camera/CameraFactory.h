#pragma once

#include "ICameraFactory.h"

namespace Dwarf
{
  class CameraFactory : public ICameraFactory
  {
  public:
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