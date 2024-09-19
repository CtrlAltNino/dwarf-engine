#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
namespace Dwarf
{
  class ISceneAssetInspector
  {
  public:
    virtual ~ISceneAssetInspector() = default;
    virtual void
    Render(IAssetReference& asset) = 0;
  };
}