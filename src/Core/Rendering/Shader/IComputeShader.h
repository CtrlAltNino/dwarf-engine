#pragma once
#include "ShaderParameterCollection.h"
#include "pch.h"

#include "IShaderParameterCollection.h"
#include "Core/Base.h"
#include "Core/UID.h"

#include <memory>

namespace Dwarf
{
  class IComputeShader
  {
  public:
    virtual ~IComputeShader();

    // Compiles the shader.
    virtual void
    Compile() = 0;

    /**
     * @brief Returns the compilation status of the shader.
     *
     * @return true if the shader has been compiled, false otherwise.
     */
    virtual bool
    IsCompiled() const = 0;

    virtual std::shared_ptr<IShaderParameterCollection>
    GetParameters() = 0;
  };
}