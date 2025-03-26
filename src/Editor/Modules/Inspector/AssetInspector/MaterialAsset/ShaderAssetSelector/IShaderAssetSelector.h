#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"
#include "Core/Rendering/Shader/IShader.h"

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
    SetCurrentShader(std::shared_ptr<IShader> shader) = 0;

    /**
     * @brief Creates a shader source collection from the current selection
     *
     * @return Unique pointer to the created shader source colleciton
     */
    virtual auto
    GetCurrentSelection() -> std::unique_ptr<IShaderSourceCollection> = 0;

    /**
     * @brief Renders the selection UI
     *
     */
    virtual void
    Render() = 0;
  };
}