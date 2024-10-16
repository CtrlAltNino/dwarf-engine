#pragma once

#include "Core/Base.h"
#include "Editor/EditorModel.h"
#include "Editor/Modules/GuiModule.h"

namespace Dwarf
{

  /// @brief Module to render a window that displays performance statistics.
  class PerformanceWindow : public GuiModule
  {
  private:
    /// @brief Pointer to the frame time.
    std::shared_ptr<double> m_Frametime = nullptr;

    /// @brief Pointer to the render time.
    std::shared_ptr<double> m_RenderTime = nullptr;

  public:
    PerformanceWindow(std::shared_ptr<EditorModel> model, int id);

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    void
    OnUpdate(double deltaTime) override;

    std::string
    Serialize() override;

    void
    Deserialize(nlohmann::json moduleData) override;
  };
}