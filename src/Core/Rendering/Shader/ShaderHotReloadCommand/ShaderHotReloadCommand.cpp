#include "pch.h"

#include "ShaderHotReloadCommand.h"

namespace Dwarf
{
  ShaderHotReloadCommand::ShaderHotReloadCommand(UUID                  id,
                                                 std::function<void()> callback)
    : mId(std::move(id))
    , mCallback(std::move(callback))
  {
  }

  void
  ShaderHotReloadCommand::Run()
  {
    mCallback();
  }

  auto
  ShaderHotReloadCommand::GetId() -> UUID
  {
    return mId;
  }
}