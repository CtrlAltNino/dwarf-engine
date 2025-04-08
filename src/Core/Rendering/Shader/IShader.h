#pragma once

#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollection.h"
#include <memory>

namespace Dwarf
{
  /**
   * @brief Class that represents a shader program and provides controls over
   * it.
   *
   */
  class IShader
  {
  public:
    virtual ~IShader() = default;

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
     * @brief Creates a ShaderParameterCollection that contains all the shader
     * parameters that the shader uses
     *
     * @return Unique pointer to the created ShaderParameterCollection
     */
    virtual auto
    CreateParameters() -> std::unique_ptr<IShaderParameterCollection> = 0;

    virtual auto
    operator<(const IShader& other) const -> bool = 0;
  };
}