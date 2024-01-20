#pragma once

#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
    class BooleanShaderParameter : public IShaderParameter
    {
    public:
        bool m_Value;

        BooleanShaderParameter()
        {
        }

        BooleanShaderParameter(bool value) : m_Value(value)
        {
        }

        virtual ShaderParameterType GetType() override
        {
            return BOOLEAN;
        }
    };
}