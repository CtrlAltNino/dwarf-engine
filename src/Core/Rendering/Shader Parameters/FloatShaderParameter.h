#pragma once

#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
    class FloatShaderParameter : public IShaderParameter
    {
    public:
        float m_Value;

        FloatShaderParameter() : m_Value(0.0f)
        {
        }

        FloatShaderParameter(float value) : m_Value(value)
        {
        }

        virtual ShaderParameterType GetType() override
        {
            return ShaderParameterType::FLOAT;
        }
    };
}