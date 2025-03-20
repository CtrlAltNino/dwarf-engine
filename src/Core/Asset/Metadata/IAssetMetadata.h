#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>

namespace Dwarf
{
  class IAssetMetadata
  {
  public:
    static constexpr const char* METADATA_EXTENSION = ".dmeta";
    virtual ~IAssetMetadata() = default;

    static auto
    GetMetadataPath(std::filesystem::path assetPath) -> std::filesystem::path
    {
      return assetPath.concat(METADATA_EXTENSION);
    }

    static auto
    IsMetadataPath(const std::filesystem::path& path) -> bool
    {
      return path.extension() == METADATA_EXTENSION;
    }

    virtual auto
    GetMetadata(const std::filesystem::path& assetPath) const
      -> nlohmann::json = 0;

    virtual void
    SetMetadata(const std::filesystem::path& assetPath,
                const nlohmann::json&        value) = 0;

    virtual void
    RemoveMetadata(const std::filesystem::path& key) = 0;

    virtual void
    Rename(const std::filesystem::path& fromPath,
           const std::filesystem::path& toPath) = 0;
  };
}