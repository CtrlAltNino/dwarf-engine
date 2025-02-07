#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
namespace Dwarf
{
  class IMaterialAssetInspector
  {
  public:
    virtual ~IMaterialAssetInspector() = default;
    virtual void
    Render(IAssetReference& asset) = 0;
  };
}