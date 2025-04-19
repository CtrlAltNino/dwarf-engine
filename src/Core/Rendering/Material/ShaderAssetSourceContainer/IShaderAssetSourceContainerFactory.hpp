#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.hpp"
#include "IShaderAssetSourceContainer.hpp"

namespace Dwarf
{
  class IShaderAssetSourceContainerFactory
  {
  public:
    virtual ~IShaderAssetSourceContainerFactory() = default;

    [[nodiscard]] virtual auto
    Create(std::unique_ptr<IShaderSourceCollection> shaderSources) const
      -> std::unique_ptr<IShaderAssetSourceContainer> = 0;
  };
}