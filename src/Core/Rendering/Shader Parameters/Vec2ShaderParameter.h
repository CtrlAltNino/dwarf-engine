#pragma once

#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
    class Vec2ShaderParameter : public IShaderParameter
    {
    public:
        glm::vec2 m_Value;

        Vec2ShaderParameter()
        {
        }

        Vec2ShaderParameter(glm::vec2 value) : m_Value(value)
        {
        }

        virtual ShaderParameterType GetType() override
        {
            return VEC2;
        }
    };
}