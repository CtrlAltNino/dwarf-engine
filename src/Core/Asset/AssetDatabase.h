#pragma once

#include <efsw/efsw.hpp>
#include <filesystem>
#include <string>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "Core/UID.h"
#include "Core/Asset/AssetReference.h"
#include "Core/Asset/AssetMetaData.h"
#include "Utilities/FileHandler.h"
#include "Core/Asset/AssetDirectoryListener.h"
#include "Core/Rendering/Shader.h"

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
  /// @brief Dwarf Engines asset database system. It handles an ECS registry
  /// containing entities for every usable Asset inside the "/Assets" directory.
  class AssetDatabase
  {
  private:
    /// @brief Absolute path to the "/Asset" directory.
    static std::filesystem::path s_AssetFolderPath;

    /// @brief Recursive file watcher for the "/Asset" directory.
    static std::shared_ptr<efsw::FileWatcher> s_FileWatcher;

    /// @brief Watch ID for the EFSW file watcher.
    static efsw::WatchID s_WatchID;

    static std::vector<std::shared_ptr<Shader>> s_ShaderRecompilationStack;

    static std::shared_ptr<AssetDirectoryListener> s_AssetDirectoryListener;

    /// @brief Recursively imports all found assets in a given directory.
    /// @param directory Absolute path to a directory.
    static void
    RecursiveImport(std::filesystem::path const& directory);

    static void
    CompileShaders();

    /// @brief Creates an asset reference for an asset at a given path.
    /// @param assetPath Path to the asset.
    /// @return The created asset reference instance.
    template<typename T>
    static AssetReference<T>
    CreateAssetReference(std::filesystem::path const& assetPath)
    {
      std::string           fileName = assetPath.stem().string();
      std::filesystem::path metaDataPath =
        assetPath.string() + AssetMetaData::META_DATA_EXTENSION;

      auto id = UID();
      if (FileHandler::CheckIfFileExists(metaDataPath.string().c_str()))
      {
        nlohmann::json metaData = AssetMetaData::GetMetaData(assetPath);
        id = UID(metaData["guid"]);
      }
      else
      {
        nlohmann::json metaData;
        metaData["guid"] = (uint64_t)id;
        AssetMetaData::SetMetaData(assetPath, metaData);
      }

      return AssetReference<T>(
        s_Registry->create(), fileName, s_Registry, id, assetPath);
    }

  public:
    static std::shared_ptr<entt::registry> s_Registry;

    static std::map<std::filesystem::path, std::shared_ptr<Shader>>
      s_ShaderAssetMap;

    static const std::map<std::string, ASSET_TYPE> s_FileAssetAssociation;

    /// @brief Initializes the asset database.
    static void
    Init(std::filesystem::path const& projectPath);

    /// @brief Clears the asset database.
    static void
    Clear();

    /// @brief Imports an asset into the asset database.
    /// @param assetPath Path to the asset.
    static std::shared_ptr<UID>
    Import(std::filesystem::path const& assetPath);

    static bool
    Exists(std::shared_ptr<UID> uid);
    static bool
    Exists(std::filesystem::path const& path);

    static void
    Rename(std::filesystem::path const& from, std::filesystem::path const& to);

    static void
    RenameDirectory(std::filesystem::path const& from,
                    std::filesystem::path const& to);

    static void
    CreateNewMaterialAsset();
    static void
    CreateNewMaterialAsset(std::filesystem::path const& path);

    static std::filesystem::path
    GetAssetDirectoryPath();

    static void
    Remove(std::shared_ptr<UID> uid);
    static void
    Remove(std::filesystem::path const& path);

    static void
    RecompileShaders();

    static void
    AddShaderWatch(std::filesystem::path const& shaderAssetPath,
                   std::shared_ptr<Shader>      shader)
    {
      s_ShaderAssetMap[shaderAssetPath] = shader;
    }

    static void
    RemoveShaderWatch(std::filesystem::path const& shaderAssetPath)
    {
      s_ShaderAssetMap.erase(shaderAssetPath);
    }

    static void
    AddShaderToRecompilationQueue(std::filesystem::path const& path)
    {
      s_ShaderRecompilationStack.push_back(s_ShaderAssetMap[path]);
    }

    static void
    AddShaderToRecompilationQueue(std::shared_ptr<Shader> shader)
    {
      s_ShaderRecompilationStack.push_back(shader);
    }

    template<typename T>
    static void
    Reimport(std::shared_ptr<AssetReference<T>> asset)
    {
      Reimport(asset->GetPath());
    }

    static std::shared_ptr<UID>
    Reimport(std::filesystem::path const& assetPath)
    {
      return AssetDatabase::Import(assetPath);
    }

    static void
    ReimportAssets();

    template<typename T>
    static std::shared_ptr<AssetReference<T>>
    Retrieve(std::shared_ptr<UID> uid)
    {
      // Retrieve entt::entity with UID component
      for (auto view = s_Registry->view<IDComponent>(); auto entity : view)
      {
        if (*view.get<IDComponent>(entity).ID == *uid)
        {
          return std::make_shared<AssetReference<T>>(
            AssetReference<T>(entity, s_Registry));
        }
      }
      return nullptr;
    }

    template<typename T>
    static std::shared_ptr<AssetReference<T>>
    Retrieve(std::filesystem::path const& path)
    {
      // Retrieve entt::entity with UID component
      for (auto view = s_Registry->view<PathComponent>(); auto entity : view)
      {
        if (view.get<PathComponent>(entity).Path == path)
        {
          return std::make_shared<AssetReference<T>>(entity, s_Registry);
        }
      }
      return nullptr;
    }

    static ASSET_TYPE
    GetType(std::filesystem::path const& assetPath)
    {
      std::string fileExtension = assetPath.extension().string();

      return s_FileAssetAssociation.at(fileExtension);
    }
  };
}