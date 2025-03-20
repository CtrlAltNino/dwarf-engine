#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"

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

    std::vector<std::unique_ptr<IAssetReference>>&
    GetShaderSources() override;
  };
}