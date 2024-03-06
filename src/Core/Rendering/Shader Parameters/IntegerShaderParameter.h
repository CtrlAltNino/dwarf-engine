#pragma once

#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
    class IntegerShaderParameter : public IShaderParameter
    {
    public:
        int m_Value;

        IntegerShaderParameter() : m_Value(0)
        {
        }

        IntegerShaderParameter(int value) : m_Value(value)
        {
        }

        virtual ShaderParameterType GetType() override
        {
            return ShaderParameterType::INTEGER;
        }
    };
}