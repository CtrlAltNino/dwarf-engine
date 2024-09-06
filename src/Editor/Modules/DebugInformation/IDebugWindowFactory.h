#pragma once

#include "Editor/Modules/DebugInformation/DebugWindow.h"
#include "Editor/Modules/IGuiModule.h"

namespace Dwarf
{
  class IDebugWindowFactory
  {
  public:
    virtual ~IDebugWindowFactory() = default;
    virtual std::shared_ptr<DebugWindow>
    Create() const = 0;

    virtual std::shared_ptr<DebugWindow>
    Create(SerializedModule serializedModule) const = 0;
  };
} // namespace Dwarf