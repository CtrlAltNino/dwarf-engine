#pragma once

#include "Editor/Modules/IGuiModule.h"

namespace Dwarf
{
  class IGuiModuleFactory
  {
  public:
    virtual ~IGuiModuleFactory() = default;

    virtual std::unique_ptr<IGuiModule>
    CreateGuiModule(MODULE_TYPE type) const = 0;

    virtual std::unique_ptr<IGuiModule>
    CreateGuiModule(SerializedModule serializedModule) const = 0;
  };
} // namespace Dwarf