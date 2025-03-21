#pragma once

#include "Core/Asset/AssetTypes.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/UUID.h"
// #include "Core/Asset/Database/AssetReference.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include <boost/algorithm/string/case_conv.hpp>
#include <entt/entity/fwd.hpp>
#include <filesystem>
#include <typeindex>
#include <boost/serialization/strong_typedef.hpp>

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
    virtual UUID
    Import(std::filesystem::path const& assetPath) = 0;

    /**
     * @brief Checks if an asset with a given UID exists in the database.
     * @param uid UID of the asset.
     */
    virtual bool
    Exists(const UUID& uid) = 0;

    /**
     * @brief Checks if an asset with a given path exists in the database.
     * @param path Path to the asset.
     */
    virtual bool
    Exists(const std::filesystem::path& path) = 0;

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
    Remove(const std::filesystem::path& path) = 0;

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
    virtual std::unique_ptr<IAssetReference>
    Retrieve(const UUID& uid) = 0;

    /**
     * @brief Retrieves an asset from the asset database.
     * @param path Path to the asset.
     */
    virtual std::unique_ptr<IAssetReference>
    Retrieve(const std::filesystem::path& path) = 0;

    virtual entt::registry&
    GetRegistry() = 0;

    /**
     * @brief Renames an asset in the asset database.
     * @param from Path to the asset.
     * @param to New path to the asset.
     */
    virtual void
    Rename(const std::filesystem::path& from,
           const std::filesystem::path& to) = 0;

    /**
     * @brief Renames a directory in the asset database.
     * @param from Old path to the directory.
     * @param to New path to the directory.
     */
    virtual void
    RenameDirectory(const std::filesystem::path& from,
                    const std::filesystem::path& to) = 0;

    static ASSET_TYPE
    GetAssetType(const std::string& extension)
    {
      auto it =
        extensionToAssetType.find(boost::algorithm::to_lower_copy(extension));
      if (it != extensionToAssetType.end())
      {
        return it->second;
      }
      else
      {
        return ASSET_TYPE::UNKNOWN;
      }
    }

    virtual std::filesystem::path
    GetAssetDirectoryPath() = 0;

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