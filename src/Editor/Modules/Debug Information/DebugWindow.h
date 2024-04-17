#pragma once

#include "Core/Base.h"
#include "Editor/EditorModel.h"
#include "Editor/Modules/GuiModule.h"

namespace Dwarf
{

  /// @brief GUI Module to display a window for the asset directory structure.
  class DebugWindow : public GuiModule
  {
  public:
    DebugWindow(Ref<EditorModel> model, int id);

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    /// @brief Executes all pre frame tasks.
    void
    OnUpdate(double deltaTime) override;

    std::string
    Serialize() override;

    void
    Deserialize(nlohmann::json moduleData) override;
  };
}