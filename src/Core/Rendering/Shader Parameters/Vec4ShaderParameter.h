#pragma once

#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
    class Vec4ShaderParameter : public IShaderParameter
    {
    public:
        glm::vec4 m_Value;

        Vec4ShaderParameter() : m_Value({0.0f, 0.0f, 0.0f, 1.0f})
        {
        }

        Vec4ShaderParameter(glm::vec4 value) : m_Value(value)
        {
        }

        virtual ShaderParameterType GetType() override
        {
            return ShaderParameterType::VEC4;
        }
    };
}