#pragma once

#include "Core/UID.h"
#include "Core/Asset/AssetReference.h"
#include "Core/Rendering/Shader.h"
#include <typeindex>

namespace Dwarf
{
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

  class IAssetDatabase
  {
  public:
    virtual ~IAssetDatabase() = default;

    // ======== Asset Management ========

    /// @brief Imports an asset into the asset database.
    /// @param assetPath Path to the asset.
    virtual std::shared_ptr<UID>
    Import(std::filesystem::path const& assetPath) = 0;

    virtual bool
    Exists(std::shared_ptr<UID> uid) = 0;
    virtual bool
    Exists(std::filesystem::path const& path) = 0;

    /// @brief Clears the asset database.
    virtual void
    Clear() = 0;
    virtual void
    Remove(std::shared_ptr<UID> uid) = 0;
    virtual void
    Remove(std::filesystem::path const& path) = 0;

    virtual void
    ReimportAll() = 0;
    virtual std::shared_ptr<UID>
    Reimport(std::filesystem::path const& assetPath) = 0;

    template<typename T>
    std::shared_ptr<AssetReference<T>>
    Retrieve(std::shared_ptr<UID> uid)
    {
      return std::static_pointer_cast<AssetReference<T>>(
        RetrieveImpl(typeid(T), uid));
    }
    template<typename T>
    std::shared_ptr<AssetReference<T>>
    Retrieve(std::filesystem::path const& path)
    {
      return std::static_pointer_cast<AssetReference<T>>(
        RetrieveImpl(typeid(T), path));
    }

    virtual void
    Rename(std::filesystem::path const& from,
           std::filesystem::path const& to) = 0;

    virtual void
    RenameDirectory(std::filesystem::path const& from,
                    std::filesystem::path const& to) = 0;

    // ======== Asset Creation ========
    virtual void
    CreateNewMaterialAsset();
    virtual void
    CreateNewMaterialAsset(std::filesystem::path const& path) = 0;

    // TODO: Move these
    virtual std::filesystem::path
    GetAssetDirectoryPath() = 0;

    virtual void
    RecompileShaders() = 0;

    virtual void
    AddShaderWatch(std::filesystem::path const& shaderAssetPath,
                   std::shared_ptr<Shader>      shader) = 0;
    virtual void
    RemoveShaderWatch(std::filesystem::path const& shaderAssetPath) = 0;

    virtual void
    AddShaderToRecompilationQueue(std::filesystem::path const& path) = 0;
    virtual void
    AddShaderToRecompilationQueue(std::shared_ptr<Shader> shader) = 0;

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
    static const std::unordered_map<std::string, ASSET_TYPE>
      extensionToAssetType;
  };

  const std::unordered_map<std::string, ASSET_TYPE>
    IAssetDatabase::extensionToAssetType = {
      { ".jpg", ASSET_TYPE::TEXTURE },
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
      { ".hlsl", ASSET_TYPE::HLSL_SHADER }
    };
}