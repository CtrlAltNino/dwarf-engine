#pragma once

#include "IShaderParameterCollection.h"

#include <memory>

namespace Dwarf
{
  class IShader
  {
  public:
    virtual ~IShader() = default;

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