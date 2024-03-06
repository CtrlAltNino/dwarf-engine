#pragma once

#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
    class Vec2ShaderParameter : public IShaderParameter
    {
    public:
        glm::vec2 m_Value;

        Vec2ShaderParameter() : m_Value({0.0f, 0.0f})
        {
        }

        Vec2ShaderParameter(glm::vec2 value) : m_Value(value)
        {
        }

        virtual ShaderParameterType GetType() override
        {
            return ShaderParameterType::VEC2;
        }
    };
}