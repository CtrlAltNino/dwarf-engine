#pragma once

#include "Core/Asset/AssetReference/IAssetReference.hpp"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.hpp"

namespace Dwarf
{
  class ShaderSourceCollection : public IShaderSourceCollection
  {
  private:
    std::vector<std::unique_ptr<IAssetReference>> mShaderSources;

  public:
    ShaderSourceCollection(
      std::vector<std::unique_ptr<IAssetReference>>& shaderSources);

    ~ShaderSourceCollection() override = default;

    /**
     * @brief Retrieves the list of the shader sources
     *
     * @return Reference to the internal vector containing the shader asset
     * references
     */
    auto
    GetShaderSources()
      -> std::vector<std::unique_ptr<IAssetReference>>& override;
  };
}