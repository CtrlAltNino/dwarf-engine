#pragma once

#include "Core/UUID.h"
namespace Dwarf
{
  class IShaderHotReloadCommand
  {
  public:
    virtual ~IShaderHotReloadCommand() = default;

    virtual void
    Run() = 0;

    virtual auto
    GetId() -> UUID = 0;
  };
}