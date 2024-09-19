#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"

namespace Dwarf
{
  class IShaderSourceCollection
  {
  public:
    virtual ~IShaderSourceCollection() = default;

    virtual std::vector<std::unique_ptr<IAssetReference>>&
    GetShaderSources() = 0;
  };
}