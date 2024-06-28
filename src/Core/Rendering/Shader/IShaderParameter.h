#pragma once

#include <typeindex>
#include "Core/Base.h"

namespace Dwarf
{
  class IShaderParameter
  {
  public:
    virtual ~IShaderParameter() = default;
    virtual std::type_index
    GetType() const = 0;
  };
}