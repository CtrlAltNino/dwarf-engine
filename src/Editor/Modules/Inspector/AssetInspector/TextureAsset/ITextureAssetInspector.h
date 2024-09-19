#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"

namespace Dwarf
{
  class ITextureAssetInspector
  {
  public:
    virtual ~ITextureAssetInspector() = default;
    virtual void
    Render(IAssetReference& asset) = 0;
  };
}