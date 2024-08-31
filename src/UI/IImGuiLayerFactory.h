#pragma once

#include "UI/IImGuiLayer.h"

namespace Dwarf
{
  class IImGuiLayerFactory
  {
  public:
    virtual ~IImGuiLayerFactory() = default;

    virtual std::shared_ptr<IImGuiLayer>
    Create() const = 0;
  };
} // namespace Dwarf