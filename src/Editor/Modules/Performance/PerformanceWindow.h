#pragma once
#include "Editor/Stats/IEditorStats.h"
#include "pch.h"
#include "Editor/Modules/IGuiModule.h"

namespace Dwarf
{

  /// @brief Module to render a window that displays performance statistics.
  class PerformanceWindow : public IGuiModule
  {
  private:
    /// @brief Pointer to the frame time.
    // std::shared_ptr<double> m_Frametime = nullptr;

    /// @brief Pointer to the render time.
    // std::shared_ptr<double> m_RenderTime = nullptr;

    std::shared_ptr<IEditorStats> m_EditorStats;

  public:
    PerformanceWindow(std::shared_ptr<IEditorStats> editorStats);
    PerformanceWindow(nlohmann::json                serializedModule,
                      std::shared_ptr<IEditorStats> editorStats);

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    void
    OnUpdate() override;

    nlohmann::json
    Serialize() const override;

    void
    Deserialize(nlohmann::json moduleData) override;
  };
}