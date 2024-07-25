#pragma once

namespace Dwarf
{
  class IAssetMetadata
  {
  private:
    static constexpr const char* METADATA_EXTENSION = ".dmeta";

  public:
    virtual ~IAssetMetadata() = default;

    static std::filesystem::path
    GetMetadataPath(std::filesystem::path assetPath)
    {
      return assetPath.concat(METADATA_EXTENSION);
    }

    static bool
    IsMetadataPath(std::filesystem::path path)
    {
      return path.extension() == METADATA_EXTENSION;
    }

    virtual nlohmann::json
    GetMetadata(const std::filesystem::path& assetPath) const = 0;
    virtual void
    SetMetadata(const std::filesystem::path& assetPath,
                const nlohmann::json&        value) = 0;
    virtual void
    RemoveMetadata(const std::filesystem::path& key) = 0;
    virtual void
    Rename(const std::filesystem::path& from,
           const std::filesystem::path& to) = 0;
  };
}