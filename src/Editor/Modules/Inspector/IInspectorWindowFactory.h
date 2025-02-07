#pragma once

#include "Editor/Modules/Inspector/InspectorWindow.h"

namespace Dwarf
{
  class IInspectorWindowFactory
  {
  public:
    virtual ~IInspectorWindowFactory() = default;
    virtual std::unique_ptr<InspectorWindow>
    Create() const = 0;

    virtual std::unique_ptr<InspectorWindow>
    Create(SerializedModule serializedModule) const = 0;
  };
} // namespace Dwarf