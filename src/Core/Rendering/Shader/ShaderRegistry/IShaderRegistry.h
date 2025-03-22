#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"
#include "Core/Rendering/Shader/IShader.h"

namespace Dwarf
{
  /**
   * @brief Class that manages reusable shader programs
   *
   */
  class IShaderRegistry
  {
  public:
    virtual ~IShaderRegistry() = default;

    /**
     * @brief Gets or creates a shader program based on source files
     *
     * @param shaderSources Source files to create the shader program from
     * @return A shared pointer to the shader program
     */
    virtual auto
    GetOrCreate(std::unique_ptr<IShaderSourceCollection> shaderSources)
      -> std::shared_ptr<IShader> = 0;
  };
}