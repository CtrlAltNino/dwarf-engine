#include "AssetMetadata.h"

namespace Dwarf
{
  AssetMetadata::AssetMetadata(std::shared_ptr<IFileHandler> fileHandler)
    : m_FileHandler(fileHandler)
  {
  }

  nlohmann::json
  AssetMetadata::GetMetadata(const std::filesystem::path& assetPath) const
  {
    std::string fileContent =
      m_FileHandler->ReadFile(IAssetMetadata::GetMetadataPath(assetPath));
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

    if (!m_FileHandler->DirectoryExists(assetPath))
    {
      m_FileHandler->CreateDirectoryAt(assetPath);
    }

    m_FileHandler->WriteToFile(GetMetadataPath(assetPath), fileContent);
  }

  /// @brief Removes the metadata file of an asset.
  /// @param assetPath Path to an asset.
  void
  AssetMetadata::RemoveMetadata(const std::filesystem::path& assetPath)
  {
    m_FileHandler->Delete(GetMetadataPath(assetPath));
  }

  void
  AssetMetadata::Rename(const std::filesystem::path& from,
                        const std::filesystem::path& to)
  {
    m_FileHandler->Rename(GetMetadataPath(from), GetMetadataPath(to));
  }
}