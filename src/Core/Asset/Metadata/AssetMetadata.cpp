#include "AssetMetadata.h"

#include <utility>

namespace Dwarf
{
  AssetMetadata::AssetMetadata(std::shared_ptr<IFileHandler> fileHandler)
    : mFileHandler(std::move(fileHandler))
  {
  }

  auto
  AssetMetadata::GetMetadata(const std::filesystem::path& assetPath) const
    -> nlohmann::json
  {
    std::string fileContent =
      mFileHandler->ReadFile(IAssetMetadata::GetMetadataPath(assetPath));
    nlohmann::json jsonObject;

    if (!fileContent.empty())
    {
      jsonObject = nlohmann::json::parse(fileContent);
    }

    return jsonObject;
  }

  /// @brief Writes the given metadata to a path.
  /// @param assetPath Path to write the metadata to.
  /// @param metaData The metadata in JSON.
  void
  AssetMetadata::SetMetadata(const std::filesystem::path& assetPath,
                             const nlohmann::json&        metaData)
  {
    std::string fileContent = metaData.dump(4);

    if (!mFileHandler->DirectoryExists(assetPath))
    {
      mFileHandler->CreateDirectoryAt(assetPath);
    }

    mFileHandler->WriteToFile(GetMetadataPath(assetPath), fileContent);
  }

  /// @brief Removes the metadata file of an asset.
  /// @param assetPath Path to an asset.
  void
  AssetMetadata::RemoveMetadata(const std::filesystem::path& assetPath)
  {
    mFileHandler->Delete(GetMetadataPath(assetPath));
  }

  void
  AssetMetadata::Rename(const std::filesystem::path& fromPath,
                        const std::filesystem::path& toPath)
  {
    mFileHandler->Rename(GetMetadataPath(fromPath), GetMetadataPath(toPath));
  }
}