#pragma once

#include "Core/Base.h"

namespace Dwarf {
  class IShaderParameter
  {
  public:
    virtual ~IShaderParameter() = default;
    virtual ShaderParameterType GetType() = 0;
  };
}