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

    /**
     * @brief Creates the metadata path for an asset path
     *
     * @param assetPath Path to an asset
     * @return Path to the metadata of an asset
     */
    static auto
    CreateMetadataPath(std::filesystem::path assetPath) -> std::filesystem::path
    {
      return assetPath.concat(METADATA_EXTENSION);
    }

    /**
     * @brief Checks if a given path is the path for a metadata file
     *
     * @param path Path to check
     * @return true If the path points to a metadata file
     * @return false If the path does not contain a metadata file
     */
    static auto
    IsMetadataPath(const std::filesystem::path& path) -> bool
    {
      return path.extension() == METADATA_EXTENSION;
    }

    /**
     * @brief Retrieves the metadata of an asset
     *
     * @param assetPath Path to an asset file
     * @return Metadata of an asset as serialized JSON
     */
    virtual auto
    GetMetadata(const std::filesystem::path& assetPath) const
      -> nlohmann::json = 0;

    /**
     * @brief Sets the metadata for an asset file
     *
     * @param assetPath Path to an asset file
     * @param metadata Metadata of the asset as serialized JSON
     */
    virtual void
    SetMetadata(const std::filesystem::path& assetPath,
                const nlohmann::json&        metadata) = 0;

    /**
     * @brief Removes the metadata file for an asset
     *
     * @param assetPath Path to an asset file
     */
    virtual void
    RemoveMetadata(const std::filesystem::path& assetPath) = 0;

    /**
     * @brief Renames/Moves the metadata
     *
     * @param fromPath Old metadata path
     * @param toPath New metadata path
     */
    virtual void
    Rename(const std::filesystem::path& fromPath,
           const std::filesystem::path& toPath) = 0;
  };
}