#pragma once

#include "Editor/Modules/IGuiModuleFactory.h"

namespace Dwarf
{
  class GuiModuleFactory : public IGuiModuleFactory
  {
  public:
    std::shared_ptr<IGuiModule>
    CreateGuiModule(MODULE_TYPE type, int index) override;

    std::shared_ptr<IGuiModule>
    CreateGuiModule(nlohmann::json serializedModule) override;
  };
} // namespace Dwarf