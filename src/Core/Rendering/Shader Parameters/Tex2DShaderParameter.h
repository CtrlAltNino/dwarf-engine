#pragma once

#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
    class Tex2DShaderParameter : public IShaderParameter
    {
    public:
        Ref<UID> m_Value;

        Tex2DShaderParameter()
        {
        }

        Tex2DShaderParameter(Ref<UID> value) : m_Value(value)
        {
        }

        virtual ShaderParameterType GetType() override
        {
            return TEX2D;
        }
    };
}