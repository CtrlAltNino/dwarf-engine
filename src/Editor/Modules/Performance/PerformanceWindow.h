#pragma once
#include "pch.h"
#include "Editor/Modules/IGuiModule.h"
#include "Editor/Stats/IEditorStats.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  /// @brief Module to render a window that displays performance statistics.
  class PerformanceWindow : public IGuiModule
  {
  private:
    std::shared_ptr<IEditorStats> m_EditorStats;

  public:
    PerformanceWindow(std::shared_ptr<IEditorStats> editorStats);

    PerformanceWindow(SerializedModule              serializedModule,
                      std::shared_ptr<IEditorStats> editorStats);

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    void
    OnUpdate() override;

    nlohmann::json
    Serialize() const override;

    void
    Deserialize(nlohmann::json moduleData);
  };
}