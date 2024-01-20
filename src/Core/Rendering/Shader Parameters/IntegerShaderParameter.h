#pragma once

#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
    class IntegerShaderParameter : public IShaderParameter
    {
    public:
        int m_Value;

        IntegerShaderParameter()
        {
        }

        IntegerShaderParameter(int value) : m_Value(value)
        {
        }

        virtual ShaderParameterType GetType() override
        {
            return INTEGER;
        }
    };
}