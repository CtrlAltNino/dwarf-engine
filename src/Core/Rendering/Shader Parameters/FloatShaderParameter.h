#pragma once

#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
    class FloatShaderParameter : public IShaderParameter
    {
    public:
        float m_Value;

        FloatShaderParameter()
        {
        }

        FloatShaderParameter(float value) : m_Value(value)
        {
        }

        virtual ShaderParameterType GetType() override
        {
            return FLOAT;
        }
    };
}