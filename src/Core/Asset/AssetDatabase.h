#pragma once

#include <filesystem>
#include <string>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include <entt/entity/registry.hpp>

#include "Core/UID.h"
#include "Utilities/FileHandler.h"
#include "AssetReference.h"
#include "AssetComponents.h"
#include "AssetDirectoryListener.h"

namespace Dwarf {
    
    /// @brief Dwarf Engines asset database system. It handles an ECS registry containing entities for every usable Asset inside the "/Assets" directory.
    class AssetDatabase : public EntityProvider {
        private:
            /// @brief Absolute path to the "/Asset" directory.
            std::filesystem::path assetFolderPath;

            /// @brief Recursive file watcher for the "/Asset" directory.
            efsw::FileWatcher * fileWatcher;

            /// @brief Watch ID for the EFSW file watcher.
            efsw::WatchID watchID;

            /// @brief Recursively imports all found assets in a given directory.
            /// @param directory Absolute path to a directory.
            void RecursiveImport(std::string directory);

            /// @brief Creates an asset reference with a given name.
            /// @param fileName Name of the Asset Reference.
            /// @return The created asset reference instance.
            AssetReference CreateAssetReference(std::string fileName);

            /// @brief Creates an asset reference with a given name and UID.
            /// @param uid UID for the asset reference.
            /// @param fileName Name of the Asset Reference.
            /// @return The created asset reference instance.
            AssetReference CreateAssetReferenceWithUID(UID uid, std::string fileName);
        public:
            AssetDatabase(std::filesystem::path projectPath);

            /// @brief Initializes the asset database.
            void Init();

            /// @brief Clears the asset database.
            void Clear();
            // AssetReference Retrieve(UID id);

            /// @brief Imports an asset into the asset database.
            /// @param assetPath Path to the asset.
            void Import(std::filesystem::path assetPath);
    };
}