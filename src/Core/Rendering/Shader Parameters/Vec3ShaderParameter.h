#pragma once

#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
    class Vec3ShaderParameter : public IShaderParameter
    {
    public:
        glm::vec3 m_Value;

        Vec3ShaderParameter() : m_Value({0.0f, 0.0f, 0.0f})
        {
        }

        Vec3ShaderParameter(glm::vec3 value) : m_Value(value)
        {
        }

        virtual ShaderParameterType GetType() override
        {
            return VEC3;
        }
    };
}