#pragma once

#include "Core/Rendering/Material/ShaderAssetSourceContainer/IShaderAssetSourceContainer.hpp"
#include "Core/Rendering/Shader/IShader.hpp"

namespace Dwarf
{
  /**
   * @brief Class that manages the selection UI for shader sources
   *
   */
  class IShaderAssetSelector
  {
  public:
    virtual ~IShaderAssetSelector() = default;

    /**
     * @brief Sets the current shader to select sources for
     *
     * @param shader Selected shader
     */
    virtual void
    SetCurrentShader(std::shared_ptr<IShader> shader,
                     std::unique_ptr<IShaderAssetSourceContainer>&
                       selectedShaderAssetSourceContainer) = 0;

    /**
     * @brief Renders the selection UI
     *
     */
    virtual void
    Render() = 0;
  };
}