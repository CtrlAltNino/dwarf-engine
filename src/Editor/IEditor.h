#pragma once

namespace Dwarf
{
  /// @brief The controller part of the editors MVC structure.
  class IEditor
  {
  public:
    IEditor() = default;
    ~IEditor() = default;

    /// @brief Starts the render loop
    virtual bool
    Run() = 0;
  };
}