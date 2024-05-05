#pragma once

#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
  class Tex2DShaderParameter : public IShaderParameter
  {
  public:
    std::shared_ptr<UID> m_Value;

    Tex2DShaderParameter()
      : m_Value(nullptr)
    {
    }

    Tex2DShaderParameter(std::shared_ptr<UID> value)
      : m_Value(value)
    {
    }

    virtual ShaderParameterType
    GetType() override
    {
      return ShaderParameterType::TEX2D;
    }
  };
}