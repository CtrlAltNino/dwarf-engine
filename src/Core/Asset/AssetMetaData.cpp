#include "AssetMetaData.h"
#include "Utilities/FileHandler.h"

namespace Dwarf
{
  nlohmann::json
  AssetMetaData::GetMetaData(const std::filesystem::path& assetPath) const
  {
    std::string fileContent =
      FileHandler::ReadFile(IAssetMetaData::GetMetaDataPath(assetPath));
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
  AssetMetaData::SetMetaData(const std::filesystem::path& assetPath,
                             const nlohmann::json&        metaData)
  {
    std::string fileContent = metaData.dump(4);

    if (!FileHandler::CheckIfDirectoyExists(assetPath))
    {
      FileHandler::CreateDirectoryAt(assetPath);
    }

    FileHandler::WriteToFile(GetMetaDataPath(assetPath), fileContent);
  }

  /// @brief Removes the metadata file of an asset.
  /// @param assetPath Path to an asset.
  void
  AssetMetaData::RemoveMetaData(const std::filesystem::path& assetPath)
  {
    FileHandler::Delete(GetMetaDataPath(assetPath));
  }

  void
  AssetMetaData::Rename(const std::filesystem::path& from,
                        const std::filesystem::path& to)
  {
    FileHandler::Rename(GetMetaDataPath(from), GetMetaDataPath(to));
  }
}