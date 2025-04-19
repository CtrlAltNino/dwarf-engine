#pragma once

#include "Core/Asset/AssetReference/IAssetReference.hpp"

namespace Dwarf
{
  class IShaderSourceCollection
  {
  public:
    virtual ~IShaderSourceCollection() = default;

    /**
     * @brief Retrieves the list of the shader sources
     *
     * @return Reference to the internal vector containing the shader asset
     * references
     */
    virtual auto
    GetShaderSources() -> std::vector<std::unique_ptr<IAssetReference>>& = 0;
  };
}