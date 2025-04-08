#pragma once

#include "Utilities/ISerializable.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  /// @brief Class that represents the editors GUI
  class IEditorView : public ISerializable
  {
  public:
    virtual ~IEditorView() = default;

    /// @brief Executes all pre frame tasks.
    virtual void
    OnUpdate() = 0;

    /// @brief Renders the GUI of the editor.
    virtual void
    OnImGuiRender() = 0;

    /**
     * @brief Shuts down the GUI
     *
     */
    virtual void
    Shutdown() = 0;
  };
}