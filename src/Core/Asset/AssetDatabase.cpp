#include "dpch.h"

#include "AssetDatabase.h"

#include <nlohmann/json.hpp>

#include "Utilities/FileHandler.h"
#include "Core/Asset/AssetComponents.h"
#include "Core/Asset/AssetDirectoryListener.h"
#include "Core/Asset/AssetMetaData.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Asset/MaterialSerializer.h"

namespace Dwarf {
    std::filesystem::path AssetDatabase::s_AssetFolderPath = "";
    efsw::FileWatcher * AssetDatabase::s_FileWatcher;
    efsw::WatchID AssetDatabase::s_WatchID;
    Ref<entt::registry> AssetDatabase::s_Registry;

    void AssetDatabase::RecursiveImport(std::filesystem::path directory){
        for(auto& directoryEntry : std::filesystem::directory_iterator(directory)){
            if(directoryEntry.is_directory()) {
                RecursiveImport(directoryEntry.path().string());
            } else if (directoryEntry.is_regular_file() && directoryEntry.path().has_extension() && directoryEntry.path().extension() != ".meta") {
                Import(directoryEntry.path());
            }
        }
    }

    /*template<typename T>
    AssetReference<T> AssetDatabase::CreateAssetReference(std::filesystem::path assetPath){
        std::string fileName = assetPath.filename().string();
        std::filesystem::path metaDataPath = assetPath.concat(".meta");

        if(FileHandler::CheckIfFileExists(metaDataPath.string().c_str())){
            nlohmann::json metaData = AssetMetaData::GetMetaData(assetPath);
            return AssetReference<T>(s_Registry->create(), fileName, s_Registry, UID(metaData["guid"]), assetPath);
        }else {
            return AssetReference<T>(s_Registry->create(), fileName, s_Registry, UID(), assetPath);
        }
    }*/

    void AssetDatabase::ReimportAssets(){
        s_Registry->clear();
        RecursiveImport(s_AssetFolderPath);
    }

    void AssetDatabase::Init(std::filesystem::path projectPath){
        s_AssetFolderPath = projectPath / "Assets";
        s_Registry = CreateRef<entt::registry>(entt::registry());

        // Create the file system watcher instance
        // efsw::FileWatcher allow a first boolean parameter that indicates if it should start with the generic file watcher instead of the platform specific backend
        s_FileWatcher = new efsw::FileWatcher();

        // Create the instance of your efsw::FileWatcherListener implementation
        AssetDirectoryListener * listener = new AssetDirectoryListener();

        // Add a folder to watch, and get the efsw::WatchID
        // It will watch the /tmp folder recursively ( the third parameter indicates that is recursive )
        // Reporting the files and directories changes to the instance of the listener
        s_WatchID = s_FileWatcher->addWatch( s_AssetFolderPath.string(), listener, true );

        // Start watching asynchronously the directories
        s_FileWatcher->watch();

        ReimportAssets();
    }

    void AssetDatabase::Clear(){
        s_FileWatcher->removeWatch( s_WatchID );
    }

    Ref<UID> AssetDatabase::Import(std::filesystem::path assetPath){
        std::string fileExtension = assetPath.extension().string();
        std::string fileName = assetPath.filename().string();
        
        if(fileExtension == ".obj" || fileExtension == ".fbx") {
            return CreateAssetReference<MeshAsset>(assetPath).GetUID();
        } else if(fileExtension == ".dmat") {
            return CreateAssetReference<MaterialAsset>(assetPath).GetUID();
        } else if(fileExtension == ".jpg" || fileExtension == ".png") {
            return CreateAssetReference<TextureAsset>(assetPath).GetUID();
        } else if(fileExtension == ".dscene") {
            return CreateAssetReference<SceneAsset>(assetPath).GetUID();
        } else if(fileExtension == ".vert") {
            return CreateAssetReference<VertexShaderAsset>(assetPath).GetUID();
        } else if(fileExtension == ".tesc") {
            return CreateAssetReference<TesselationControlShaderAsset>(assetPath).GetUID();
        } else if(fileExtension == ".tese") {
            return CreateAssetReference<TesselationEvaluationShaderAsset>(assetPath).GetUID();
        } else if(fileExtension == ".geom") {
            return CreateAssetReference<GeometryShaderAsset>(assetPath).GetUID();
        } else if(fileExtension == ".frag") {
            return CreateAssetReference<FragmentShaderAsset>(assetPath).GetUID();
        } else if(fileExtension == ".comp") {
            return CreateAssetReference<ComputeShaderAsset>(assetPath).GetUID();
        } else if(fileExtension == ".metal"){
            return CreateAssetReference<MetalShaderAsset>(assetPath).GetUID();
        } else if(fileExtension == ".hlsl") {
            return CreateAssetReference<HlslShaderAsset>(assetPath).GetUID();
        } else{
            return CreateAssetReference<UnknownAsset>(assetPath).GetUID();
        }
    }

    bool AssetDatabase::Exists(Ref<UID> uid){
        // Retrieve entt::entity with UID component
        auto view = s_Registry->view<IDComponent>();
        for(auto entity : view){
            if(view.get<IDComponent>(entity).ID == uid){
                return true;
            }
        }
        return false;
    }

    bool AssetDatabase::Exists(std::filesystem::path path){
        // Retrieve entt::entity with UID component
        auto view = s_Registry->view<PathComponent>();
        for(auto entity : view){
            if(view.get<PathComponent>(entity).Path == path){
                return true;
            }
        }
        return false;
    }

    void AssetDatabase::CreateNewMaterialAsset(){
        CreateNewMaterialAsset(s_AssetFolderPath);
    }

    void AssetDatabase::CreateNewMaterialAsset(std::filesystem::path path){
        Material newMat = Material("New Material");
        std::filesystem::path newMatPath = path / "New Material.dmat";
        MaterialSerializer::Serialize(newMat, newMatPath);
    }

    std::filesystem::path AssetDatabase::GetAssetDirectoryPath(){
        return s_AssetFolderPath;
    }
}