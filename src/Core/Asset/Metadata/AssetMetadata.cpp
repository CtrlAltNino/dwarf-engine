#include "pch.h"

#include "AssetMetadata.h"
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
      mFileHandler->ReadFile(IAssetMetadata::CreateMetadataPath(assetPath));
    nlohmann::json jsonObject;

    if (!fileContent.empty())
    {
      jsonObject = nlohmann::json::parse(fileContent);
    }

    return jsonObject;
  }

  void
  AssetMetadata::SetMetadata(const std::filesystem::path& assetPath,
                             const nlohmann::json&        metaData)
  {
    std::string fileContent = metaData.dump(4);

    if (!mFileHandler->DirectoryExists(assetPath))
    {
      mFileHandler->CreateDirectoryAt(assetPath);
    }

    mFileHandler->WriteToFile(CreateMetadataPath(assetPath), fileContent);
  }

  void
  AssetMetadata::RemoveMetadata(const std::filesystem::path& assetPath)
  {
    mFileHandler->Delete(CreateMetadataPath(assetPath));
  }

  void
  AssetMetadata::Rename(const std::filesystem::path& fromPath,
                        const std::filesystem::path& toPath)
  {
    mFileHandler->Rename(CreateMetadataPath(fromPath),
                         CreateMetadataPath(toPath));
  }
}