#pragma once

#include "Core/UID.h"
#include "Core/Asset/Database/AssetReference.h"
#include <entt/entity/fwd.hpp>
#include <typeindex>

namespace Dwarf
{
  /**
   * @brief Enum class representing the type of an asset.
   */
  enum class ASSET_TYPE
  {
    UNKNOWN,
    MODEL,
    TEXTURE,
    SCENE,
    MATERIAL,
    VERTEX_SHADER,
    TESC_SHADER,
    TESE_SHADER,
    GEOMETRY_SHADER,
    FRAGMENT_SHADER,
    COMPUTE_SHADER,
    HLSL_SHADER
  };

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
    virtual std::shared_ptr<UID>
    Import(std::filesystem::path const& assetPath) = 0;

    /**
     * @brief Checks if an asset with a given UID exists in the database.
     * @param uid UID of the asset.
     */
    virtual bool
    Exists(std::shared_ptr<UID> uid) = 0;

    /**
     * @brief Checks if an asset with a given path exists in the database.
     * @param path Path to the asset.
     */
    virtual bool
    Exists(std::filesystem::path const& path) = 0;

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
    Remove(std::shared_ptr<UID> uid) = 0;

    /**
     * @brief Removes an asset from the asset database.
     * @param path Path to the asset.
     */
    virtual void
    Remove(std::filesystem::path const& path) = 0;

    /**
     * @brief Reimports all assets in the asset database.
     */
    virtual void
    ReimportAll() = 0;

    /**
     * @brief Reimports an asset in the asset database.
     * @param assetPath Path to the asset.
     */
    virtual std::shared_ptr<UID>
    Reimport(std::filesystem::path const& assetPath) = 0;

    /**
     * @brief Retrieves an asset from the asset database.
     * @param uid UID of the asset.
     */
    template<typename T>
    std::shared_ptr<AssetReference<T>>
    Retrieve(std::shared_ptr<UID> uid)
    {
      return std::static_pointer_cast<AssetReference<T>>(
        RetrieveImpl(typeid(T), uid));
    }

    /**
     * @brief Retrieves an asset from the asset database.
     * @param path Path to the asset.
     */
    template<typename T>
    std::shared_ptr<AssetReference<T>>
    Retrieve(std::filesystem::path const& path)
    {
      return std::static_pointer_cast<AssetReference<T>>(
        RetrieveImpl(typeid(T), path));
    }

    virtual std::shared_ptr<entt::registry>
    GetRegistry() = 0;

    /**
     * @brief Renames an asset in the asset database.
     * @param from Path to the asset.
     * @param to New path to the asset.
     */
    virtual void
    Rename(std::filesystem::path const& from,
           std::filesystem::path const& to) = 0;

    /**
     * @brief Renames a directory in the asset database.
     * @param from Old path to the directory.
     * @param to New path to the directory.
     */
    virtual void
    RenameDirectory(std::filesystem::path const& from,
                    std::filesystem::path const& to) = 0;

    static ASSET_TYPE
    GetAssetType(const std::string& extension)
    {
      auto it = extensionToAssetType.find(extension);
      if (it != extensionToAssetType.end())
      {
        return it->second;
      }
      else
      {
        return ASSET_TYPE::UNKNOWN;
      }
    }

  private:
    // NVI (Non-Virtual Interface) Implementations
    virtual std::shared_ptr<void>
    RetrieveImpl(std::type_index type, std::shared_ptr<UID> uid) = 0;

    virtual std::shared_ptr<void>
    RetrieveImpl(std::type_index type, std::filesystem::path const& path) = 0;

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