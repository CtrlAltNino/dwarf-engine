#pragma once

#include "DI/IDwarfEditorDI.h"
#include "Editor/Modules/IGuiModuleFactory.h"

namespace Dwarf
{
  class GuiModuleFactory : public IGuiModuleFactory
  {
  private:
    // std::shared_ptr<IDwarfEditorDI> m_EditorDI;
    boost::di::injector<IEditor>& m_EditorDI;

  public:
    explicit GuiModuleFactory(boost::di::injector<IEditor>& editorDI);

    std::shared_ptr<IGuiModule>
    CreateGuiModule(MODULE_TYPE type) override;

    std::shared_ptr<IGuiModule>
    CreateGuiModule(nlohmann::json serializedModule) override;
  };
} // namespace Dwarf