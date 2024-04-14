#include "dpch.h"

#include "AssetDatabase.h"

#include <nlohmann/json.hpp>

#include "Utilities/FileHandler.h"
#include "Core/Asset/AssetComponents.h"
#include "Core/Asset/AssetMetaData.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Asset/MaterialSerializer.h"

namespace Dwarf {
  std::filesystem::path AssetDatabase::s_AssetFolderPath = "";
  Ref<efsw::FileWatcher> AssetDatabase::s_FileWatcher;
  Ref<AssetDirectoryListener> AssetDatabase::s_AssetDirectoryListener = nullptr;
  efsw::WatchID AssetDatabase::s_WatchID;
  Ref<entt::registry> AssetDatabase::s_Registry;
  std::map<std::filesystem::path, Ref<Shader>> AssetDatabase::s_ShaderAssetMap;
  std::vector<Ref<Shader>> AssetDatabase::s_ShaderRecompilationStack;
  const std::map<std::string, ASSET_TYPE>
    AssetDatabase::s_FileAssetAssociation = {
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

  void AssetDatabase::RecursiveImport(std::filesystem::path const& directory)
  {
    for (auto& directoryEntry :
         std::filesystem::directory_iterator(directory)) {
      if (directoryEntry.is_directory()) {
        RecursiveImport(directoryEntry.path().string());
      } else if (directoryEntry.is_regular_file() &&
                 directoryEntry.path().has_extension() &&
                 directoryEntry.path().extension() !=
                   AssetMetaData::META_DATA_EXTENSION) {
        Import(directoryEntry.path());
      }
    }
  }

  void AssetDatabase::ReimportAssets()
  {
    s_Registry->clear();
    Shader::Init();
    ComputeShader::Init();
    Material::Init();
    Mesh::Init();
    RecursiveImport(s_AssetFolderPath);
  }

  void AssetDatabase::Remove(Ref<UID> uid)
  {
    auto view = s_Registry->view<IDComponent>();
    for (auto entity : view) {
      if (*view.get<IDComponent>(entity).ID == *uid) {
        s_Registry->destroy(entity);
      }
    }
  }
  void AssetDatabase::Remove(std::filesystem::path const& path)
  {
    auto view = s_Registry->view<PathComponent>();
    for (auto entity : view) {
      if (view.get<PathComponent>(entity).Path == path &&
          s_Registry->valid(entity)) {
        s_Registry->destroy(entity);
      }
    }
  }

  void AssetDatabase::Init(std::filesystem::path const& projectPath)
  {
    s_AssetFolderPath = projectPath / "Assets";

    if (!FileHandler::CheckIfDirectoyExists(s_AssetFolderPath)) {
      FileHandler::CreateDirectory(s_AssetFolderPath);
    }
    s_Registry = CreateRef<entt::registry>(entt::registry());

    // Create the file system watcher instance
    // efsw::FileWatcher allow a first boolean parameter that indicates if it
    // should start with the generic file watcher instead of the platform
    // specific backend
    s_FileWatcher = CreateRef<efsw::FileWatcher>();

    // Create the instance of your efsw::FileWatcherListener implementation
    s_AssetDirectoryListener = CreateRef<AssetDirectoryListener>();

    // Add a folder to watch, and get the efsw::WatchID
    // It will watch the /tmp folder recursively ( the third parameter indicates
    // that is recursive ) Reporting the files and directories changes to the
    // instance of the listener
    s_WatchID = s_FileWatcher->addWatch(
      s_AssetFolderPath.string(), s_AssetDirectoryListener.get(), true);
    s_WatchID = s_FileWatcher->addWatch(Shader::GetDefaultShaderPath().string(),
                                        s_AssetDirectoryListener.get(),
                                        true);

    // Start watching asynchronously the directories
    s_FileWatcher->watch();

    ReimportAssets();

    CompileShaders();
  }

  void AssetDatabase::CompileShaders()
  {
    auto materialView = AssetDatabase::s_Registry->view<MaterialAsset>();

    for (auto entity : materialView) {
      s_Registry->get<MaterialAsset>(entity).m_Material->GetShader()->Compile();
    }
  }

  void AssetDatabase::Clear()
  {
    s_FileWatcher->removeWatch(s_WatchID);
  }

  Ref<UID> AssetDatabase::Import(std::filesystem::path const& assetPath)
  {
    std::string fileName = assetPath.filename().string();
    ASSET_TYPE assetType = GetType(assetPath);

    // Remove asset if already present
    if (AssetDatabase::Exists(assetPath)) {
      AssetDatabase::Remove(assetPath);
    }

    switch (assetType) {
      using enum ASSET_TYPE;
      case MODEL: {
        AssetReference<Dwarf::ModelAsset> model =
          CreateAssetReference<ModelAsset>(assetPath);
        model.GetAsset()->Load();
        return model.GetUID();
      }
      case MATERIAL: {
        return CreateAssetReference<MaterialAsset>(assetPath).GetUID();
      }
      case TEXTURE: {
        return CreateAssetReference<TextureAsset>(assetPath).GetUID();
      }
      case SCENE: {
        return CreateAssetReference<SceneAsset>(assetPath).GetUID();
      }
      case VERTEX_SHADER: {
        return CreateAssetReference<VertexShaderAsset>(assetPath).GetUID();
      }
      case TESC_SHADER: {
        return CreateAssetReference<TesselationControlShaderAsset>(assetPath)
          .GetUID();
      }
      case TESE_SHADER: {
        return CreateAssetReference<TesselationEvaluationShaderAsset>(assetPath)
          .GetUID();
      }
      case GEOMETRY_SHADER: {
        return CreateAssetReference<GeometryShaderAsset>(assetPath).GetUID();
      }
      case FRAGMENT_SHADER: {
        return CreateAssetReference<FragmentShaderAsset>(assetPath).GetUID();
      }
      case COMPUTE_SHADER: {
        return CreateAssetReference<ComputeShaderAsset>(assetPath).GetUID();
      }
      case HLSL_SHADER: {
        return CreateAssetReference<HlslShaderAsset>(assetPath).GetUID();
      }
      default: {
        return CreateAssetReference<UnknownAsset>(assetPath).GetUID();
      }
    }
  }

  bool AssetDatabase::Exists(Ref<UID> uid)
  {
    if (uid) {
      // Retrieve entt::entity with UID component
      auto view = s_Registry->view<IDComponent>();
      for (auto entity : view) {
        if (*view.get<IDComponent>(entity).ID == *uid) {
          return true;
        }
      }
    }
    return false;
  }

  bool AssetDatabase::Exists(std::filesystem::path const& path)
  {
    // Retrieve entt::entity with UID component
    auto view = s_Registry->view<PathComponent>();
    for (auto entity : view) {
      if (view.get<PathComponent>(entity).Path == path) {
        return true;
      }
    }
    return false;
  }

  void AssetDatabase::Rename(std::filesystem::path const& from,
                             std::filesystem::path const& to)
  {
    AssetMetaData::Rename(from, to);
    auto view = s_Registry->view<PathComponent, NameComponent>();
    auto matView = s_Registry->view<MaterialAsset>();
    for (auto entity : view) {
      if (view.get<PathComponent>(entity).Path == from) {
        s_Registry->remove<PathComponent>(entity);
        s_Registry->remove<NameComponent>(entity);
        s_Registry->emplace<PathComponent>(entity, to);
        s_Registry->emplace<NameComponent>(entity, to.stem().string());

        if (matView.contains(entity)) {
          matView.get<MaterialAsset>(entity).m_Material->m_Name =
            to.stem().string();
        }
        break;
      }
    }
  }

  void AssetDatabase::RenameDirectory(std::filesystem::path const& from,
                                      std::filesystem::path const& to)
  {
    auto view = s_Registry->view<PathComponent>();
    for (auto entity : view) {
      if (view.get<PathComponent>(entity).Path.string().find(from.string()) !=
          std::string::npos) {
        std::filesystem::path newPath = to;
        newPath.concat(view.get<PathComponent>(entity).Path.string().erase(
          0, from.string().length()));
        s_Registry->remove<PathComponent>(entity);
        s_Registry->emplace<PathComponent>(entity, newPath);
      }
    }
  }

  void AssetDatabase::CreateNewMaterialAsset()
  {
    CreateNewMaterialAsset(s_AssetFolderPath);
  }

  void AssetDatabase::CreateNewMaterialAsset(std::filesystem::path const& path)
  {
    auto newMat = Material("New Material");
    newMat.GenerateShaderParameters();
    std::filesystem::path newMatPath = path / "New Material.dmat";

    while (FileHandler::CheckIfFileExists(newMatPath)) {
      std::filesystem::path fileNameWithoutExtension =
        newMatPath.filename().replace_extension("");
      std::string lastPart = fileNameWithoutExtension.string();

      size_t pos = 0;
      std::string token;
      while ((pos = lastPart.find(" ")) != std::string::npos) {
        lastPart.erase(0, pos + 1);
      }

      bool isNumber = true;

      for (char c : lastPart) {
        if (!std::isdigit(c)) {
          isNumber = false;
        }
      }

      if (isNumber) {
        int val = stoi(lastPart);
        val++;

        newMatPath.replace_filename(
          fileNameWithoutExtension.string().substr(
            0, fileNameWithoutExtension.string().length() - lastPart.length()) +
          std::to_string(val) + std::string(".dmat"));
      } else {
        newMatPath.replace_filename(fileNameWithoutExtension.string() +
                                    std::string(" 2") + std::string(".dmat"));
      }
    }
    MaterialSerializer::Serialize(newMat, newMatPath);
  }

  std::filesystem::path AssetDatabase::GetAssetDirectoryPath()
  {
    return s_AssetFolderPath;
  }

  void AssetDatabase::RecompileShaders()
  {
    for (Ref<Shader> shader : s_ShaderRecompilationStack) {
      shader->Compile();
    }
    s_ShaderRecompilationStack.clear();
  }
}