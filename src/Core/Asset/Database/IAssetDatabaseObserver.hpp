#pragma once

#include "Core/Asset/AssetTypes.hpp"
#include "Core/UUID.hpp"
namespace Dwarf
{
  class IAssetDatabaseObserver
  {
  public:
    virtual ~IAssetDatabaseObserver() = default;

    virtual void
    OnReimportAll() = 0;

    virtual void
    OnReimportAsset(const std::filesystem::path& assetPath,
                    ASSET_TYPE                   assetType,
                    const UUID&                  uid) = 0;

    virtual void
    OnImportAsset(const std::filesystem::path& assetPath,
                  ASSET_TYPE                   assetType,
                  const UUID&                  uid) = 0;

    virtual void
    OnAssetDatabaseClear() = 0;

    virtual void
    OnRemoveAsset(const std::filesystem::path& path) = 0;

    virtual void
    OnRename(const std::filesystem::path& oldPath,
             const std::filesystem::path& newPath) = 0;
  };
}