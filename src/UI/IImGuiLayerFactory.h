#pragma once

#include "UI/IImGuiLayer.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  class IImGuiLayerFactory
  {
  public:
    virtual ~IImGuiLayerFactory() = default;

    virtual std::unique_ptr<IImGuiLayer>
    Create() const = 0;
  };
} // namespace Dwarf