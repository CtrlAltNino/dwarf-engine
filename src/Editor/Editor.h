#pragma once
#include "pch.h"
#include "Window/Window.h"
#include "Core/UI/ImGuiLayer.h"
#include "Editor/IEditorModel.h"
#include "Editor/IEditorView.h"
#include "Editor/IEditor.h"

namespace Dwarf
{
  struct ProjectPath
  {
    std::filesystem::path value;
    constexpr
    operator std::filesystem::path() const
    {
      return value;
    }
  };
  /// @brief The controller part of the editors MVC structure.
  class Editor : public IEditor
  {
  private:
    /// @brief The MVC model instance of this editor instance.
    std::unique_ptr<IEditorModel> m_Model;

    /// @brief The MVC view instance of this editor instance.
    std::unique_ptr<IEditorView> m_View;

    std::unique_ptr<Window> m_Window;

    std::unique_ptr<ImGuiLayer> m_ImguiLayer;

    void
    Init();

  public:
    Editor(const ProjectPath& projectPath);
    ~Editor();

    /// @brief Starts the render loop
    bool
    Run() override;

    void
    UpdateWindowTitle() const override;
  };
}