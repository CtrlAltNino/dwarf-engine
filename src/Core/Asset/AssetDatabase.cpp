#include "Core/Asset/AssetDatabase.h"

namespace Dwarf {

    AssetDatabase::AssetDatabase(std::filesystem::path projectPath) : assetFolderPath(projectPath / "Assets") {}

    void AssetDatabase::Init(){
        // Create the file system watcher instance
        // efsw::FileWatcher allow a first boolean parameter that indicates if it should start with the generic file watcher instead of the platform specific backend
        this->fileWatcher = new efsw::FileWatcher();

        // Create the instance of your efsw::FileWatcherListener implementation
        AssetDirectoryListener * listener = new AssetDirectoryListener();

        // Add a folder to watch, and get the efsw::WatchID
        // It will watch the /tmp folder recursively ( the third parameter indicates that is recursive )
        // Reporting the files and directories changes to the instance of the listener
        watchID = this->fileWatcher->addWatch( assetFolderPath.string(), listener, true );

        // Start watching asynchronously the directories
        this->fileWatcher->watch();

        RecursiveImport(assetFolderPath.string());
    }

    void AssetDatabase::RecursiveImport(std::string directory) {
        for(auto& directoryEntry : std::filesystem::directory_iterator(directory)){
            if(directoryEntry.is_directory()) {
                RecursiveImport(directoryEntry.path().string());
            } else if (directoryEntry.is_regular_file()) {
                Import(directoryEntry.path());
            }
        }
    }

    void AssetDatabase::Clear(){
        this->fileWatcher->removeWatch( this->watchID );
    }

    /*AssetReference AssetDatabase::Retrieve(UID id){
        return AssetReference();
    }*/

    AssetReference AssetDatabase::CreateAssetReference(std::string fileName){
        return CreateAssetReferenceWithUID(UID(), fileName);
    }

    AssetReference AssetDatabase::CreateAssetReferenceWithUID(UID uid, std::string fileName){
        AssetReference assetReference = { m_Registry.create(), this };
        assetReference.AddComponent<IDComponent>(uid);
        auto& tag = assetReference.AddComponent<TagComponent>(fileName);
        tag.Tag = fileName.empty() ? "Asset" : fileName;

        //entity.SetParent(rootEntity.GetHandle());

        return assetReference;
    }

    void AssetDatabase::Import(std::filesystem::path assetPath) {
        std::string fileExtension = assetPath.extension().string();
        std::string fileName = assetPath.filename().string();
        std::cout << "Trying to import file with extension [" << fileExtension << "] found at <" + assetPath.string() << ">" << std::endl;
        // Get meta data
        //nlohmann::json metaData = 

        if(fileExtension == ".obj" || fileExtension == ".fbx") {
            // Create asset reference with a mesh asset component
            AssetReference newAssetReference = CreateAssetReference(fileName);
            newAssetReference.AddComponent<PathComponent>(assetPath);
            newAssetReference.AddComponent<MeshAsset>(assetPath);
        } else if(fileExtension == ".dmat") {
            // Create asset reference with a material asset component
            AssetReference newAssetReference = CreateAssetReference(fileName);
            newAssetReference.AddComponent<PathComponent>(assetPath);
            newAssetReference.AddComponent<MaterialAsset>(assetPath);
        } else if(fileExtension == ".jpg" || fileExtension == ".png") {
            // Create asset reference with a texture asset component
            AssetReference newAssetReference = CreateAssetReference(fileName);
            newAssetReference.AddComponent<PathComponent>(assetPath);
            newAssetReference.AddComponent<TextureAsset>(assetPath);
        } else if(fileExtension == ".dscene") {
            // Create asset reference with a scene asset component
            AssetReference newAssetReference = CreateAssetReference(fileName);
            newAssetReference.AddComponent<PathComponent>(assetPath);
            newAssetReference.AddComponent<SceneAsset>(assetPath);
        } else if(fileExtension == ".vert") {
            // Create asset reference with a vertex shader asset component
            AssetReference newAssetReference = CreateAssetReference(fileName);
            newAssetReference.AddComponent<PathComponent>(assetPath);
            newAssetReference.AddComponent<VertexShaderAsset>(assetPath);
        } else if(fileExtension == ".tesc") {
            // Create asset reference with a tesselation control shader asset component
            AssetReference newAssetReference = CreateAssetReference(fileName);
            newAssetReference.AddComponent<PathComponent>(assetPath);
            newAssetReference.AddComponent<TesselationControlShaderAsset>(assetPath);
        } else if(fileExtension == ".tese") {
            // Create asset reference with a tesselation evaluation shader asset component
            AssetReference newAssetReference = CreateAssetReference(fileName);
            newAssetReference.AddComponent<PathComponent>(assetPath);
            newAssetReference.AddComponent<TesselationEvaluationShaderAsset>(assetPath);
        } else if(fileExtension == ".geom") {
            // Create asset reference with a geometry shader asset component
            AssetReference newAssetReference = CreateAssetReference(fileName);
            newAssetReference.AddComponent<PathComponent>(assetPath);
            newAssetReference.AddComponent<GeometryShaderAsset>(assetPath);
        } else if(fileExtension == ".frag") {
            // Create asset reference with a fragment shader asset component
            AssetReference newAssetReference = CreateAssetReference(fileName);
            newAssetReference.AddComponent<PathComponent>(assetPath);
            newAssetReference.AddComponent<FragmentShaderAsset>(assetPath);
        } else if(fileExtension == ".comp") {
            // Create asset reference with a compute shader asset component
            AssetReference newAssetReference = CreateAssetReference(fileName);
            newAssetReference.AddComponent<PathComponent>(assetPath);
            newAssetReference.AddComponent<ComputeShaderAsset>(assetPath);
        }
        //return AssetReference();
    }
}