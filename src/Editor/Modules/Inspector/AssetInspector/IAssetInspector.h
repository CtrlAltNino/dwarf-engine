#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
namespace Dwarf
{
  class IAssetInspector
  {
  public:
    virtual ~IAssetInspector() = default;
    virtual void
    Render() = 0;
  };
}