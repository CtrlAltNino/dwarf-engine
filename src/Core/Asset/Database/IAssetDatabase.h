#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/AssetTypes.h"
#include "Core/UUID.h"
#include <boost/serialization/strong_typedef.hpp>
#include <entt/entity/fwd.hpp>
#include <filesystem>

namespace Dwarf
{
  BOOST_STRONG_TYPEDEF(std::filesystem::path, AssetDirectoryPath);

  /**
   * @brief Interface for the Asset Database.
   */
  class IAssetDatabase
  {
  public:
    virtual ~IAssetDatabase() = default;

    // ======== Asset Management ========

    /**
     * @brief Imports an asset into the asset database.
     * @param assetPath Path to the asset.
     */
    virtual auto
    Import(std::filesystem::path const& assetPath) -> UUID = 0;

    /**
     * @brief Opens a Dialog to import assets
     */
    virtual void
    ImportDialog() = 0;

    /**
     * @brief Checks if an asset with a given UID exists in the database.
     * @param uid UID of the asset.
     */
    virtual auto
    Exists(const UUID& uid) -> bool = 0;

    /**
     * @brief Checks if an asset with a given path exists in the database.
     * @param path Path to the asset.
     */
    virtual auto
    Exists(const std::filesystem::path& assetPath) -> bool = 0;

    /**
     * @brief Clears the asset database.
     */
    virtual void
    Clear() = 0;

    /**
     * @brief Removes an asset from the asset database.
     * @param uid UID of the asset.
     */
    virtual void
    Remove(const UUID& uid) = 0;

    /**
     * @brief Removes an asset from the asset database.
     * @param path Path to the asset.
     */
    virtual void
    Remove(const std::filesystem::path& assetPath) = 0;

    /**
     * @brief Reimports all assets in the asset database.
     */
    virtual void
    ReimportAll() = 0;

    /**
     * @brief Reimports an asset in the asset database.
     * @param assetPath Path to the asset.
     */
    virtual void
    Reimport(const std::filesystem::path& assetPath) = 0;

    /**
     * @brief Retrieves an asset from the asset database.
     * @param uid UID of the asset.
     */
    virtual auto
    Retrieve(const UUID& uid) -> std::unique_ptr<IAssetReference> = 0;

    /**
     * @brief Retrieves an asset from the asset database.
     * @param path Path to the asset.
     */
    virtual auto
    Retrieve(const std::filesystem::path& assetPath)
      -> std::unique_ptr<IAssetReference> = 0;

    /**
     * @brief Returns a reference to the underlying entt registry
     *
     * @return entt::registry reference
     */
    virtual auto
    GetRegistry() -> entt::registry& = 0;

    /**
     * @brief Renames an asset in the asset database.
     * @param from Path to the asset.
     * @param to New path to the asset.
     */
    virtual void
    Rename(const std::filesystem::path& fromPath,
           const std::filesystem::path& toPath) = 0;

    /**
     * @brief Renames a directory in the asset database.
     * @param from Old path to the directory.
     * @param to New path to the directory.
     */
    virtual void
    RenameDirectory(const std::filesystem::path& fromPath,
                    const std::filesystem::path& toPath) = 0;

    /**
     * @brief Retrieves the asset type enum value corresponding to a file
     * extension
     *
     * @param extension File extensions like ".jpg" or ".png"
     * @return ASSET_TYPE enum value
     */
    static auto
    GetAssetType(const std::string& extension) -> ASSET_TYPE
    {
      auto extensionIterator =
        extensionToAssetType.find(boost::algorithm::to_lower_copy(extension));

      if (extensionIterator != extensionToAssetType.end())
      {
        return extensionIterator->second;
      }

      return ASSET_TYPE::UNKNOWN;
    }

    /**
     * @brief Returns the path to the projects asset directory
     *
     * @return The path of the asset directory
     */
    virtual auto
    GetAssetDirectoryPath() -> std::filesystem::path = 0;

  protected:
    static inline const std::unordered_map<std::string, ASSET_TYPE>
      extensionToAssetType = { { ".jpg", ASSET_TYPE::TEXTURE },
                               { ".jpeg", ASSET_TYPE::TEXTURE },
                               { ".png", ASSET_TYPE::TEXTURE },
                               { ".bmp", ASSET_TYPE::TEXTURE },
                               { ".tga", ASSET_TYPE::TEXTURE },
                               { ".hdr", ASSET_TYPE::TEXTURE },
                               { ".exr", ASSET_TYPE::TEXTURE },
                               { ".tiff", ASSET_TYPE::TEXTURE },
                               { ".tif", ASSET_TYPE::TEXTURE },

                               { ".obj", ASSET_TYPE::MODEL },
                               { ".fbx", ASSET_TYPE::MODEL },
                               { ".gltf", ASSET_TYPE::MODEL },
                               { ".glb", ASSET_TYPE::MODEL },

                               { ".dmat", ASSET_TYPE::MATERIAL },
                               { ".dscene", ASSET_TYPE::SCENE },

                               { ".vert", ASSET_TYPE::VERTEX_SHADER },
                               { ".tesc", ASSET_TYPE::TESC_SHADER },
                               { ".tese", ASSET_TYPE::TESE_SHADER },
                               { ".geom", ASSET_TYPE::GEOMETRY_SHADER },
                               { ".frag", ASSET_TYPE::FRAGMENT_SHADER },
                               { ".comp", ASSET_TYPE::COMPUTE_SHADER },
                               { ".hlsl", ASSET_TYPE::HLSL_SHADER } };
  };
}