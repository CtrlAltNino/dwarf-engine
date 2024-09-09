#pragma once

#include "Editor/Modules/IGuiModule.h"
#include "Editor/Modules/Performance/PerformanceWindow.h"

namespace Dwarf
{
  class IPerformanceWindowFactory
  {
  public:
    virtual ~IPerformanceWindowFactory() = default;
    virtual std::unique_ptr<PerformanceWindow>
    Create() const = 0;

    virtual std::unique_ptr<PerformanceWindow>
    Create(SerializedModule serializedModule) const = 0;
  };
} // namespace Dwarf