#pragma once

#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf {
  class UnsignedIntegerShaderParameter : public IShaderParameter
  {
  public:
    uint32_t m_Value;

    UnsignedIntegerShaderParameter()
      : m_Value(0)
    {
    }

    UnsignedIntegerShaderParameter(uint32_t value)
      : m_Value(value)
    {
    }

    virtual ShaderParameterType GetType() override
    {
      return ShaderParameterType::UNSIGNED_INTEGER;
    }
  };
}