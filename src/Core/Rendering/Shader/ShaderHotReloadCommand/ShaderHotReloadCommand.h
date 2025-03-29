#pragma once

#include "IShaderHotReloadCommand.h"
namespace Dwarf
{
  class ShaderHotReloadCommand : public IShaderHotReloadCommand
  {
  private:
    UUID                  mId;
    std::function<void()> mCallback;

  public:
    ShaderHotReloadCommand(UUID id, std::function<void()> callback);
    ~ShaderHotReloadCommand() override = default;

    void
    Run() override;

    auto
    GetId() -> UUID override;
  };
}