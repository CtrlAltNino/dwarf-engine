#pragma once

#include "Core/Scene/IScene.h"
namespace Dwarf
{
  /// @brief The controller part of the editors MVC structure.
  class IEditor
  {
  public:
    virtual ~IEditor() = default;

    /*
     * @brief Starts the editor.
     * @return True if the user wants to return to the project launcher after
     * closing the editor.
     */
    virtual auto
    Run() -> bool = 0;
  };
}