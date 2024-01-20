#pragma once

#include "Core/Base.h"

namespace Dwarf
{
    class IShaderParameter
    {
    public:
        virtual ShaderParameterType GetType() = 0;
    };
}