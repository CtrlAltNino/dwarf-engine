#pragma once
#include "Logging/IDwarfLogger.h"
#include "pch.h"
#include "Window/Window.h"
#include "Core/UI/ImGuiLayer.h"
#include "Editor/IEditorModel.h"
#include "Editor/IEditorView.h"
#include "Editor/IEditor.h"

namespace Dwarf
{
  /// @brief The controller part of the editors MVC structure.
  class Editor : public IEditor
  {
  private:
    /// @brief The MVC model instance of this editor instance.
    std::shared_ptr<IEditorModel> m_Model;

    /// @brief The MVC view instance of this editor instance.
    std::shared_ptr<IEditorView> m_View;

    std::shared_ptr<Window> m_Window;

    std::shared_ptr<ImGuiLayer>   m_ImguiLayer;
    std::shared_ptr<IDwarfLogger> m_Logger;

    void
    Init();

  public:
    Editor(const std::shared_ptr<IDwarfLogger>& logger);

    /// @brief Starts the render loop
    bool
    Run() override;

    void
    UpdateWindowTitle() const override;
  };
}