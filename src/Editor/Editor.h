#pragma once

#include "Window/Window.h"

#include "Core/Base.h"
#include "Editor/EditorModel.h"
#include "Editor/EditorView.h"
#include "Core/UI/ImGuiLayer.h"

namespace Dwarf
{

  /// @brief The controller part of the editors MVC structure.
  class Editor
  {
  private:
    static std::shared_ptr<Editor> s_Instance;
    /// @brief The MVC model instance of this editor instance.
    std::shared_ptr<EditorModel> m_Model;

    /// @brief The MVC view instance of this editor instance.
    std::shared_ptr<EditorView> m_View;

    std::shared_ptr<Window> m_Window;

    std::shared_ptr<ImGuiLayer> m_ImguiLayer;

    void
    Init(std::filesystem::path const& projectPath);

  public:
    Editor();
    ~Editor();

    static std::shared_ptr<Editor>
    Get()
    {
      return s_Instance;
    }

    std::shared_ptr<EditorModel>
    GetModel() const
    {
      return m_Model;
    }

    /// @brief Starts the render loop
    bool
    Run(std::filesystem::path const& projectPath);

    void
    UpdateWindowTitle() const;
  };

  std::shared_ptr<Editor>
  CreateEditor();
}