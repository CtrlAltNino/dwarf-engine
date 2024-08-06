#pragma once
#include "Editor/IEditor.h"

namespace Dwarf
{
  /// @brief View part of the editor's MVC structure.
  class IEditorView
  {
  public:
    // explicit IEditorView() = default;
    virtual ~IEditorView() = default;

    /// @brief Executes all pre frame tasks.
    virtual void
    OnUpdate() = 0;

    /// @brief Renders the GUI of the editor.
    virtual void
    OnImGuiRender() = 0;

    virtual void
    DockWindowToFocused() = 0;
  };
}