#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
namespace Dwarf
{
  class IModelAssetInspector
  {
  public:
    virtual ~IModelAssetInspector() = default;
    virtual void
    Render(IAssetReference& asset) = 0;
  };
}