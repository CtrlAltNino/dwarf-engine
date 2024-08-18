#include "AssetMetadata.h"
#include "Utilities/FileHandler.h"

namespace Dwarf
{
  nlohmann::json
  AssetMetadata::GetMetadata(const std::filesystem::path& assetPath) const
  {
    std::string fileContent =
      FileHandler::ReadFile(IAssetMetadata::GetMetadataPath(assetPath));
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

    if (!FileHandler::DirectoyExists(assetPath))
    {
      FileHandler::CreateDirectoryAt(assetPath);
    }

    FileHandler::WriteToFile(GetMetadataPath(assetPath), fileContent);
  }

  /// @brief Removes the metadata file of an asset.
  /// @param assetPath Path to an asset.
  void
  AssetMetadata::RemoveMetadata(const std::filesystem::path& assetPath)
  {
    FileHandler::Delete(GetMetadataPath(assetPath));
  }

  void
  AssetMetadata::Rename(const std::filesystem::path& from,
                        const std::filesystem::path& to)
  {
    FileHandler::Rename(GetMetadataPath(from), GetMetadataPath(to));
  }
}