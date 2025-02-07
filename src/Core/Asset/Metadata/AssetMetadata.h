#pragma once

#include "IAssetMetadata.h"
#include "Utilities/FileHandler/IFileHandler.h"
namespace Dwarf
{

  /// @brief Utilities for Reading and writing meta data.
  class AssetMetadata : public IAssetMetadata
  {
  private:
    std::shared_ptr<IFileHandler> m_FileHandler;

  public:
    AssetMetadata(std::shared_ptr<IFileHandler> fileHandler);
    ~AssetMetadata() override = default;
    /// @brief Retrieves the meta data from an asset path.
    /// @param assetPath Path to an asset.
    /// @return Metadata in JSON.
    nlohmann::json
    GetMetadata(const std::filesystem::path& assetPath) const override;

    /// @brief Writes the given metadata to a path.
    /// @param assetPath Path to write the metadata to.
    /// @param metaData The metadata in JSON.
    void
    SetMetadata(const std::filesystem::path& assetPath,
                const nlohmann::json&        metaData) override;

    /// @brief Removes the metadata file of an asset.
    /// @param assetPath Path to an asset.
    void
    RemoveMetadata(const std::filesystem::path& assetPath) override;

    void
    Rename(const std::filesystem::path& from,
           const std::filesystem::path& to) override;
  };
}