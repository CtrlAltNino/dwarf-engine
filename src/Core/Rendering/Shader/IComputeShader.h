#pragma once

#include "ShaderParameterCollection/IShaderParameterCollection.h"

namespace Dwarf
{
  /**
   * @brief Class that represents a compute shader program and provides controls
   * over it.
   *
   */
  class IComputeShader
  {
  public:
    virtual ~IComputeShader() = default;

    /**
     * @brief Compiles the shader program
     *
     */
    virtual void
    Compile() = 0;

    /**
     * @brief Returns the compilation status of the shader.
     *
     * @return true if the shader has been compiled, false otherwise.
     */
    [[nodiscard]] virtual auto
    IsCompiled() const -> bool = 0;

    /**
     * @brief Gets the parameters for the ShaderParameterCollection
     *
     * @return Reference to the ShaderParameterCollection
     */
    virtual auto
    GetParameters() -> std::shared_ptr<IShaderParameterCollection> = 0;
  };
}