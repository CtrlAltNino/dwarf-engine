#pragma once

#include "Editor/Modules/IGuiModule.h"

namespace Dwarf
{
  class IGuiModuleFactory
  {
  public:
    virtual ~IGuiModuleFactory() = default;

    virtual std::shared_ptr<IGuiModule>
    CreateGuiModule(MODULE_TYPE type, int index) = 0;

    virtual std::shared_ptr<IGuiModule>
    CreateGuiModule(nlohmann::json serializedModule) = 0;
  };
} // namespace Dwarf