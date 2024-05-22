#pragma once
#include "pch.h"
#include "Logging/EditorLogger.h"
#include "Window/Window.h"
#include "Core/UI/ImGuiLayer.h"
// #include "Editor/IEditorModel.h"
#include "Editor/IEditorView.h"
#include "Editor/IEditor.h"
#include "Editor/IEditorStats.h"
#include "Input/IInputManager.h"

namespace Dwarf
{
  /// @brief The controller part of the editors MVC structure.
  class Editor : public IEditor
  {
  private:
    /// @brief The MVC model instance of this editor instance.
    // std::shared_ptr<IEditorModel> m_Model;

    /// @brief The MVC view instance of this editor instance.
    std::shared_ptr<IEditorView> m_View;

    std::shared_ptr<Window> m_Window;

    // std::shared_ptr<ImGuiLayer>   m_ImguiLayer;
    std::shared_ptr<EditorLogger>  m_Logger;
    std::shared_ptr<IEditorStats>  m_Stats;
    std::shared_ptr<IInputManager> m_InputManager;

    void
    Init();

  public:
    Editor(const std::shared_ptr<EditorLogger>&  logger,
           const std::shared_ptr<IEditorStats>&  stats,
           const std::shared_ptr<IInputManager>& inputManager);

    /// @brief Starts the render loop
    bool
    Run() override;

    // void
    // UpdateWindowTitle() const override;
  };
}