#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"
#include "IShader.h"

namespace Dwarf
{
  /**
   * @brief Class that constructs Shader instances depending on the used
   * graphics API
   *
   */
  class IShaderFactory
  {
  public:
    virtual ~IShaderFactory() = default;

    /**
     * @brief Creates a shader instance bases on provided shader sources
     *
     * @param shaderSources Shader sources to create the shader from
     * @return Shared pointer to the created shader
     */
    [[nodiscard]] virtual auto
    Create(std::unique_ptr<IShaderSourceCollection> shaderSources) const
      -> std::shared_ptr<IShader> = 0;
  };
} // namespace Dwarf