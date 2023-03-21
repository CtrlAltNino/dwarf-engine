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

namespace Dwarf {
    
    /// @brief Dwarf Engines asset database system. It handles an ECS registry containing entities for every usable Asset inside the "/Assets" directory.
    class AssetDatabase {
        private:
            /// @brief Absolute path to the "/Asset" directory.
            static std::filesystem::path s_AssetFolderPath;

            /// @brief Recursive file watcher for the "/Asset" directory.
            static efsw::FileWatcher * s_FileWatcher;

            /// @brief Watch ID for the EFSW file watcher.
            static efsw::WatchID s_WatchID;

            /// @brief Recursively imports all found assets in a given directory.
            /// @param directory Absolute path to a directory.
            static void RecursiveImport(std::filesystem::path directory);

            /// @brief Creates an asset reference for an asset at a given path.
            /// @param assetPath Path to the asset.
            /// @return The created asset reference instance.
            template<typename T>
            static AssetReference<T> CreateAssetReference(std::filesystem::path assetPath){
                std::string fileName = assetPath.stem().string();
                std::filesystem::path metaDataPath = assetPath.string() + ".meta";

                UID id = UID();
                if(FileHandler::CheckIfFileExists(metaDataPath.string().c_str())){
                    nlohmann::json metaData = AssetMetaData::GetMetaData(assetPath);
                    id = UID(metaData["guid"]);
                }else{
                    nlohmann::json metaData;
                    metaData["guid"] = (uint64_t)id;
                    AssetMetaData::SetMetaData(assetPath, metaData);
                }

                return AssetReference<T>(s_Registry->create(), fileName, s_Registry, id, assetPath);
            }
        public:
            static Ref<entt::registry> s_Registry;

            /// @brief Initializes the asset database.
            static void Init(std::filesystem::path projectPath);

            /// @brief Clears the asset database.
            static void Clear();

            /// @brief Imports an asset into the asset database.
            /// @param assetPath Path to the asset.
            static Ref<UID> Import(std::filesystem::path assetPath);

            static bool Exists(Ref<UID> uid);
            static bool Exists(std::filesystem::path path);

            static void CreateNewMaterialAsset();
            static void CreateNewMaterialAsset(std::filesystem::path path);

            static std::filesystem::path GetAssetDirectoryPath();

            static void ReimportAssets();

            template<typename T>
            static Ref<AssetReference<T>> Retrieve(Ref<UID> uid){
                // Retrieve entt::entity with UID component
                auto view = s_Registry->view<IDComponent>();
                for(auto entity : view){
                    if(*view.get<IDComponent>(entity).ID == *uid){
                        return CreateRef<AssetReference<T>>(AssetReference<T>(entity, s_Registry));
                    }
                }
                return nullptr;
            }

            template<typename T>
            static Ref<AssetReference<T>> Retrieve(std::filesystem::path path){
                // Retrieve entt::entity with UID component
                auto view = s_Registry->view<PathComponent>();
                for(auto entity : view){
                    if(view.get<PathComponent>(entity).Path == path){
                        return CreateRef<AssetReference<T>>(AssetReference<T>(entity, s_Registry));
                    }
                }
                return nullptr;
            }
    };
}