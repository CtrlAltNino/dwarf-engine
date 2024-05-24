#pragma once

#include "IAssetMetaData.h"
namespace Dwarf
{

  /// @brief Utilities for Reading and writing meta data.
  class AssetMetaData : public IAssetMetaData
  {
  public:
    /// @brief Retrieves the meta data from an asset path.
    /// @param assetPath Path to an asset.
    /// @return Metadata in JSON.
    nlohmann::json
    GetMetaData(const std::filesystem::path& assetPath) const override;

    /// @brief Writes the given metadata to a path.
    /// @param assetPath Path to write the metadata to.
    /// @param metaData The metadata in JSON.
    void
    SetMetaData(const std::filesystem::path& assetPath,
                const nlohmann::json&        metaData) override;

    /// @brief Removes the metadata file of an asset.
    /// @param assetPath Path to an asset.
    void
    RemoveMetaData(const std::filesystem::path& assetPath) override;

    void
    Rename(const std::filesystem::path& from,
           const std::filesystem::path& to) override;
  };
}