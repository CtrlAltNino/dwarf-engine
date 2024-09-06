#pragma once

#include "Editor/Modules/Inspector/InspectorWindow.h"

namespace Dwarf
{
  class IInspectorWindowFactory
  {
  public:
    virtual ~IInspectorWindowFactory() = default;
    virtual std::shared_ptr<InspectorWindow>
    Create() const = 0;

    virtual std::shared_ptr<InspectorWindow>
    Create(SerializedModule serializedModule) const = 0;
  };
} // namespace Dwarf