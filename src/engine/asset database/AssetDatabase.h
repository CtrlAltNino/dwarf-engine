#pragma once
#include<entt/entt.hpp>

#include"../editor/UID.h"
#include <nlohmann/json.hpp>
#include "../../utilities/FileHandler.h"
#include "AssetReference.h"
#include<entt/entity/registry.hpp>
#include "AssetComponents.h"
#include "AssetDirectoryListener.h"
#include<filesystem>

class AssetDatabase : public EntityProvider {
    private:
        std::filesystem::path assetFolderPath;
        efsw::FileWatcher * fileWatcher;
        efsw::WatchID watchID;

        void RecursiveImport(std::string directory);
        AssetReference CreateAssetReference(std::string fileName);
        AssetReference CreateAssetReferenceWithUID(UID uid, std::string fileName);
    public:
        AssetDatabase(std::filesystem::path projectPath);

        void Init();
        void Clear();
        // AssetReference Retrieve(UID id);
        void Import(std::filesystem::path assetPath);
};