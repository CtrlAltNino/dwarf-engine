#pragma once

#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Database/AssetReference.h"
namespace Dwarf
{
  class IMaterialPreview
  {
  public:
    virtual void
    RenderMaterialPreview(
      std::shared_ptr<AssetReference<MaterialAsset>> materialAsset) = 0;
  };
}