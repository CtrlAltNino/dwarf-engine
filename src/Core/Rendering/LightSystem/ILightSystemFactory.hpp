#pragma once

#include "Core/Rendering/LightSystem/ILightSystem.hpp"

namespace Dwarf
{
  class ILightSystemFactory
  {
  public:
    virtual ~ILightSystemFactory() = default;

    virtual auto
    Create() -> std::unique_ptr<ILightSystem> = 0;
  };
}