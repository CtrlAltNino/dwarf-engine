#pragma once
#include "pch.h"
#include "Window/IWindow.h"
#include "Editor/Modules/GuiModule.h"
#include "Editor/EditorView/IEditorView.h"
// #include "Editor/IEditorModel.h"

namespace Dwarf
{
  /// @brief View part of the editor's MVC structure.
  class EditorView : public IEditorView
  {
  private:
    // std::unique_ptr<IEditorModel>& m_Model;
    std::shared_ptr<IWindow> m_Window;

    /// @brief ID counter for GUI modules.
    int m_GuiModuleIDCount = 0;

    /// @brief List of GUI modules.
    std::vector<std::shared_ptr<GuiModule>> m_GuiModules;

    /// @brief IMGUI example function to render the base docking layout.
    void
    RenderDockSpace();

    /// @brief Adds a new IMGUI window for a GUI module.
    /// @param moduleType Type of module to add a window for.
    void
    AddWindow(MODULE_TYPE moduleType);

    /// @brief Removes a IMGUI window that corresponds to a given index.
    /// @param index Index of the window to remove.
    void
    RemoveWindow(int index);

    void
    UpdateWindowTitle() const;

  public:
    explicit EditorView(std::shared_ptr<IWindow> window);

    /// @brief Initializes the view.
    // void
    // Init();

    /// @brief Executes all pre frame tasks.
    void
    OnUpdate() override;

    /// @brief Renders the GUI of the editor.
    void
    OnImGuiRender() override;

    void
    DockWindowToFocused() override;
  };
}