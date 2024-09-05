#pragma once
#include "Editor/IEditor.h"
#include "Utilities/ISerializable.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  BOOST_STRONG_TYPEDEF(std::optional<nlohmann::json>, SerializedView);
  /// @brief View part of the editor's MVC structure.
  class IEditorView : public ISerializable
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