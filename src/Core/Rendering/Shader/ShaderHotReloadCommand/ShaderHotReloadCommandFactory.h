#pragma once

#include "IShaderHotReloadCommand.h"
#include "IShaderHotReloadCommandFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class ShaderHotReloadCommandFactory : public IShaderHotReloadCommandFactory
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;

  public:
    ShaderHotReloadCommandFactory(std::shared_ptr<IDwarfLogger> logger);
    ~ShaderHotReloadCommandFactory() override;

    [[nodiscard]] auto
    Create(UUID id, std::function<void()> callback) const
      -> std::unique_ptr<IShaderHotReloadCommand> override;
  };
}