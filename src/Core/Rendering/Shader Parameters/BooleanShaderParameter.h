#pragma once

#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
  class BooleanShaderParameter : public IShaderParameter
  {
  public:
    bool m_Value;

    BooleanShaderParameter()
      : m_Value(false)
    {
    }

    BooleanShaderParameter(bool value)
      : m_Value(value)
    {
    }

    virtual ShaderParameterType
    GetType() override
    {
      return ShaderParameterType::BOOLEAN;
    }
  };
}