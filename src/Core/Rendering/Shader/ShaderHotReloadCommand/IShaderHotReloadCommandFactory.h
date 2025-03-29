#pragma once

#include "IShaderHotReloadCommand.h"

namespace Dwarf
{
  class IShaderHotReloadCommandFactory
  {
  public:
    virtual ~IShaderHotReloadCommandFactory() = default;

    [[nodiscard]] virtual auto
    Create(UUID id, std::function<void()> callback) const
      -> std::unique_ptr<IShaderHotReloadCommand> = 0;
  };
}