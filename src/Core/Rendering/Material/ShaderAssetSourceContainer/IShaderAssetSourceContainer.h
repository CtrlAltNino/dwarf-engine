#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"

namespace Dwarf
{
  class IShaderAssetSourceContainer
  {
  public:
    virtual ~IShaderAssetSourceContainer() = default;

    virtual auto
    GetShaderSources() -> std::unique_ptr<IShaderSourceCollection> = 0;
  };
}