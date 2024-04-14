#pragma once

#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf {
  class Tex2DShaderParameter : public IShaderParameter
  {
  public:
    Ref<UID> m_Value;

    Tex2DShaderParameter()
      : m_Value(nullptr)
    {
    }

    Tex2DShaderParameter(Ref<UID> value)
      : m_Value(value)
    {
    }

    virtual ShaderParameterType GetType() override
    {
      return ShaderParameterType::TEX2D;
    }
  };
}