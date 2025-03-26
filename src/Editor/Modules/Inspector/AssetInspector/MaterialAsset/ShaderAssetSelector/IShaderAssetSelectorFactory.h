#pragma once

#include "IShaderAssetSelector.h"

namespace Dwarf
{
  /**
   * @brief Factory class that creates ShaderAssetSelector instances
   *
   */
  class IShaderAssetSelectorFactory
  {
  public:
    virtual ~IShaderAssetSelectorFactory() = default;

    /**
     * @brief Creates a ShaderAssetSelector instance based on the used graphics
     * api
     *
     * @return Unique pointer to the created instance
     */
    virtual auto
    Create() -> std::unique_ptr<IShaderAssetSelector> = 0;
  };
}