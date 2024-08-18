#pragma once
#include "pch.h"
#include "Editor/EditorModel.h"
#include "Editor/Modules/IGuiModule.h"

namespace Dwarf
{

  /// @brief Module to render a window that displays performance statistics.
  class PerformanceWindow : public IGuiModule
  {
  private:
    /// @brief Pointer to the frame time.
    std::shared_ptr<double> m_Frametime = nullptr;

    /// @brief Pointer to the render time.
    std::shared_ptr<double> m_RenderTime = nullptr;

  public:
    PerformanceWindow(int id);

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    void
    OnUpdate(double deltaTime) override;

    nlohmann::json
    Serialize() const override;

    void
    Deserialize(nlohmann::json moduleData) override;
  };
}