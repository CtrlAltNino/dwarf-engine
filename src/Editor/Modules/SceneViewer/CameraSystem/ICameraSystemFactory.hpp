#pragma once

#include "ICameraSystem.hpp"
namespace Dwarf
{
  class ICameraSystemFactory
  {
  public:
    virtual ~ICameraSystemFactory() = default;

    virtual auto
    Create() -> std::shared_ptr<ICameraSystem> = 0;
  };
}