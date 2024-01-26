#pragma once

#include <efsw/efsw.hpp>
#include <filesystem>
#include <string>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "Core/Base.h"
#include "Core/UID.h"
#include "Core/Asset/AssetReference.h"
#include "Core/Asset/AssetMetaData.h"
#include "Core/Asset/AssetComponents.h"
#include "Core/Scene/SceneComponents.h"
#include "Utilities/FileHandler.h"

namespace Dwarf
{
    enum ASSET_TYPE
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
    /// @brief Dwarf Engines asset database system. It handles an ECS registry containing entities for every usable Asset inside the "/Assets" directory.
    class AssetDatabase
    {
    private:
        /// @brief Absolute path to the "/Asset" directory.
        static std::filesystem::path s_AssetFolderPath;

        /// @brief Recursive file watcher for the "/Asset" directory.
        static efsw::FileWatcher *s_FileWatcher;

        /// @brief Watch ID for the EFSW file watcher.
        static efsw::WatchID s_WatchID;

        static std::vector<Shader *> s_ShaderRecompilationStack;

        /// @brief Recursively imports all found assets in a given directory.
        /// @param directory Absolute path to a directory.
        static void RecursiveImport(std::filesystem::path directory);

        static void CompileShaders();

        /// @brief Creates an asset reference for an asset at a given path.
        /// @param assetPath Path to the asset.
        /// @return The created asset reference instance.
        template <typename T>
        static AssetReference<T> CreateAssetReference(std::filesystem::path assetPath)
        {
            std::string fileName = assetPath.stem().string();
            std::filesystem::path metaDataPath = assetPath.string() + AssetMetaData::META_DATA_EXTENSION;

            UID id = UID();
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

            return AssetReference<T>(s_Registry->create(), fileName, s_Registry, id, assetPath);
        }

    public:
        static Ref<entt::registry> s_Registry;

        static std::map<std::filesystem::path, Shader *> s_ShaderAssetMap;

        /// @brief Initializes the asset database.
        static void Init(std::filesystem::path projectPath);

        /// @brief Clears the asset database.
        static void Clear();

        /// @brief Imports an asset into the asset database.
        /// @param assetPath Path to the asset.
        static Ref<UID> Import(std::filesystem::path assetPath);

        static bool Exists(Ref<UID> uid);
        static bool Exists(std::filesystem::path path);

        static void Rename(std::filesystem::path from, std::filesystem::path to);

        static void CreateNewMaterialAsset();
        static void CreateNewMaterialAsset(std::filesystem::path path);

        static std::filesystem::path GetAssetDirectoryPath();

        static void Remove(Ref<UID> uid);
        static void Remove(std::filesystem::path path);

        static void RecompileShaders();

        static void AddShaderWatch(std::filesystem::path shaderAssetPath, Shader *shader)
        {
            s_ShaderAssetMap[shaderAssetPath] = shader;
        }

        static void RemoveShaderWatch(std::filesystem::path shaderAssetPath)
        {
            s_ShaderAssetMap.erase(shaderAssetPath);
        }

        static void AddShaderToRecompilationQueue(std::filesystem::path path)
        {
            s_ShaderRecompilationStack.push_back(s_ShaderAssetMap[path]);
        }

        static void AddShaderToRecompilationQueue(Shader *shader)
        {
            s_ShaderRecompilationStack.push_back(shader);
        }

        template <typename T>
        static void Reimport(Ref<AssetReference<T>> asset)
        {
            Reimport(asset->GetPath());
        }

        static Ref<UID> Reimport(std::filesystem::path assetPath)
        {
            AssetDatabase::Remove(assetPath);
            return AssetDatabase::Import(assetPath);
        }

        static void ReimportAssets();

        template <typename T>
        static Ref<AssetReference<T>> Retrieve(Ref<UID> uid)
        {
            // Retrieve entt::entity with UID component
            auto view = s_Registry->view<IDComponent>();
            for (auto entity : view)
            {
                if (*view.get<IDComponent>(entity).ID == *uid)
                {
                    return CreateRef<AssetReference<T>>(AssetReference<T>(entity, s_Registry));
                }
            }
            return nullptr;
        }

        template <typename T>
        static Ref<AssetReference<T>> Retrieve(std::filesystem::path path)
        {
            // Retrieve entt::entity with UID component
            auto view = s_Registry->view<PathComponent>();
            for (auto entity : view)
            {
                if (view.get<PathComponent>(entity).Path == path)
                {
                    return CreateRef<AssetReference<T>>(AssetReference<T>(entity, s_Registry));
                }
            }
            return nullptr;
        }

        static ASSET_TYPE GetType(std::filesystem::path assetPath)
        {
            if (assetPath.extension() == ".fbx")
            {
                return ASSET_TYPE::MODEL;
            }
            else if (assetPath.extension() == ".obj")
            {
                return ASSET_TYPE::MODEL;
            }
            else if (assetPath.extension() == ".jpg" || assetPath.extension() == ".png" || assetPath.extension() == ".tga")
            {
                return ASSET_TYPE::TEXTURE;
            }
            else if (assetPath.extension() == ".vert")
            {
                return ASSET_TYPE::VERTEX_SHADER;
            }
            else if (assetPath.extension() == ".tesc")
            {
                return ASSET_TYPE::TESC_SHADER;
            }
            else if (assetPath.extension() == ".tese")
            {
                return ASSET_TYPE::TESE_SHADER;
            }
            else if (assetPath.extension() == ".geom")
            {
                return ASSET_TYPE::GEOMETRY_SHADER;
            }
            else if (assetPath.extension() == ".frag")
            {
                return ASSET_TYPE::FRAGMENT_SHADER;
            }
            else if (assetPath.extension() == ".comp")
            {
                return ASSET_TYPE::COMPUTE_SHADER;
            }
            else if (assetPath.extension() == ".hlsl")
            {
                return ASSET_TYPE::HLSL_SHADER;
            }
            else if (assetPath.extension() == ".dscene")
            {
                return ASSET_TYPE::SCENE;
            }
            else if (assetPath.extension() == ".dmat")
            {
                return ASSET_TYPE::MATERIAL;
            }
            else
            {
                return ASSET_TYPE::UNKNOWN;
            }
        }
    };
}