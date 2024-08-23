#pragma once

#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Database/AssetReference.h"
#include "Core/Rendering/Material/IMaterial.h"
namespace Dwarf
{
  class IMaterialService
  {
  public:
    virtual ~IMaterialService() = default;

    virtual void
    SaveMaterial(std::shared_ptr<IMaterial> material) = 0;

    virtual std::shared_ptr<IMaterial>
    LoadMaterial(
      std::shared_ptr<AssetReference<MaterialAsset>> materialAsset) = 0;

    virtual std::shared_ptr<IMaterial>
    CreateNewMaterialAsset(std::optional<std::filesystem::path> assetPath) = 0;
  };
} // namespace Dwarf