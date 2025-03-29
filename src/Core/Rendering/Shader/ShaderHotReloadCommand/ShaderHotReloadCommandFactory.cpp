#include "ShaderHotReloadCommand.h"
#include "pch.h"

#include "ShaderHotReloadCommandFactory.h"

namespace Dwarf
{
  ShaderHotReloadCommandFactory::(std::shared_ptr<IDwarfLogger> logger)
    : mLogger(std::move(logger))
  {
    mLogger->LogDebug(Log("ShaderHotReloadCommandFactory created",
                          "ShaderHotReloadCommandFactory"));
  }
  ShaderHotReloadCommandFactory::~ShaderHotReloadCommandFactory()
  {
    mLogger->LogDebug(Log("ShaderHotReloadCommandFactory created",
                          "ShaderHotReloadCommandFactory"));
  }

  auto
  ShaderHotReloadCommandFactory::Create(UUID id, std::function<void()> callback)
    const -> std::unique_ptr<IShaderHotReloadCommand>
  {
    return std::make_unique<ShaderHotReloadCommand>(id, callback);
  }
}