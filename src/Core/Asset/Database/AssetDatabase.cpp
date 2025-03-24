#include "pch.h"

#include "AssetDatabase.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "Core/Base.h"
#include "Core/GenericComponents.h"
#include "IAssetDatabase.h"

namespace Dwarf
{
  AssetDatabase::AssetDatabase(
    const AssetDirectoryPath&                assetDirectoryPath,
    GraphicsApi                              graphicsApi,
    std::shared_ptr<IDwarfLogger>            logger,
    std::shared_ptr<IAssetDirectoryListener> assetDirectoryListener,
    std::shared_ptr<IAssetMetadata>          assetMetadata,
    std::shared_ptr<IModelImporter>          modelImporter,
    std::shared_ptr<IShaderRecompiler>       shaderRecompiler,
    std::shared_ptr<ITextureFactory>         textureFactory,
    std::shared_ptr<IMaterialFactory>        materialFactory,
    std::shared_ptr<IMaterialIO>             materialIO,
    std::shared_ptr<IAssetReimporter>        assetReimporter,
    std::shared_ptr<IAssetReferenceFactory>  assetReferenceFactory,
    std::shared_ptr<IFileHandler>            fileHandler,
    std::shared_ptr<IWindow>                 window)
    : mAssetDirectoryPath(assetDirectoryPath)
    , mGraphicsApi(graphicsApi)
    , mLogger(std::move(logger))
    , mAssetDirectoryListener(std::move(assetDirectoryListener))
    , mAssetMetadata(std::move(assetMetadata))
    , mModelImporter(std::move(modelImporter))
    , mShaderRecompiler(std::move(shaderRecompiler))
    , mTextureFactory(std::move(textureFactory))
    , mMaterialFactory(std::move(materialFactory))
    , mMaterialIO(std::move(materialIO))
    , mAssetReimporter(std::move(assetReimporter))
    , mAssetReferenceFactory(std::move(assetReferenceFactory))
    , mFileHandler(std::move(fileHandler))
  {
    if (!mFileHandler->DirectoryExists(mAssetDirectoryPath.t))
    {
      mFileHandler->CreateDirectoryAt(mAssetDirectoryPath.t);
    }

    ImportDefaultAssets();

    mAssetDirectoryListener->registerAddFileCallback(
      std::bind(&AssetDatabase::AddAssetCallback,
                this,
                std::placeholders::_1,
                std::placeholders::_2));
    mAssetDirectoryListener->registerDeleteFileCallback(
      std::bind(&AssetDatabase::DeleteAssetCallback,
                this,
                std::placeholders::_1,
                std::placeholders::_2));
    mAssetDirectoryListener->registerModifyFileCallback(
      std::bind(&AssetDatabase::ModifyAssetCallback,
                this,
                std::placeholders::_1,
                std::placeholders::_2));
    mAssetDirectoryListener->registerMoveFileCallback(
      std::bind(&AssetDatabase::MoveAssetCallback,
                this,
                std::placeholders::_1,
                std::placeholders::_2,
                std::placeholders::_3));
  }

  AssetDatabase::~AssetDatabase()
  {
    Clear();
  }

  void
  AssetDatabase::GatherAssetPaths(
    const std::filesystem::path&        directory,
    std::vector<std::filesystem::path>& materialPaths,
    std::vector<std::filesystem::path>& otherPaths)
  {

    for (const auto& directoryEntry :
         std::filesystem::directory_iterator(directory))
    {
      if (directoryEntry.is_directory())
      {
        GatherAssetPaths(directoryEntry.path(), materialPaths, otherPaths);
      }
      else if (directoryEntry.is_regular_file() &&
               directoryEntry.path().has_extension() &&
               !IAssetMetadata::IsMetadataPath(directoryEntry))
      {
        if (directoryEntry.path().has_extension() &&
            directoryEntry.path().extension() == ".dmat")
        {
          materialPaths.push_back(directoryEntry.path());
        }
        else
        {
          otherPaths.push_back(directoryEntry.path());
        }
      }
    }
  }

  void
  AssetDatabase::ReimportAll()
  {
    // CLearing database
    // Clear();

    // Reimporting default assets
    // ImportDefaultAssets();

    std::vector<std::filesystem::path> materialPaths = {};
    std::vector<std::filesystem::path> otherPaths = {};

    // Reimporting all assets from the asset directory
    // RecursiveImport(mAssetDirectoryPath.t);

    GatherAssetPaths(mAssetDirectoryPath, materialPaths, otherPaths);

    for (auto& path : otherPaths)
    {
      Import(path);
    }

    for (auto& path : materialPaths)
    {
      Import(path);
    }
  }

  void
  AssetDatabase::Reimport(const std::filesystem::path& assetPath)
  {
    if (Exists(assetPath))
    {
      std::unique_ptr<IAssetReference> asset = Retrieve(assetPath);

      switch (asset->GetType())
      {
        case ASSET_TYPE::MODEL:
          {
            mRegistry.emplace_or_replace<ModelAsset>(
              asset->GetHandle(), mModelImporter->Import(assetPath));
            break;
          }
        case ASSET_TYPE::TEXTURE:
          {
            // mRegistry.replace<TextureAsset>(
            //   asset->GetHandle(),
            //   mTextureFactory->FromPath(assetPath),
            //   mTextureFactory->GetPlaceholderTexture());

            mRegistry.get<TextureAsset>(asset->GetHandle()).SetTexture(nullptr);
            break;
          }
        case ASSET_TYPE::MATERIAL:
          {
            mRegistry.emplace_or_replace<MaterialAsset>(
              asset->GetHandle(), mMaterialIO->LoadMaterial(assetPath));
            break;
          }
        case ASSET_TYPE::COMPUTE_SHADER:
          {
            mRegistry.emplace_or_replace<ComputeShaderAsset>(
              asset->GetHandle(), mFileHandler->ReadFile(assetPath));
            break;
          }
        case ASSET_TYPE::FRAGMENT_SHADER:
          {
            mRegistry.emplace_or_replace<FragmentShaderAsset>(
              asset->GetHandle(), mFileHandler->ReadFile(assetPath));
            break;
          }
        case ASSET_TYPE::GEOMETRY_SHADER:
          {
            mRegistry.emplace_or_replace<GeometryShaderAsset>(
              asset->GetHandle(), mFileHandler->ReadFile(assetPath));
            break;
          }
        case ASSET_TYPE::HLSL_SHADER:
          {
            mRegistry.emplace_or_replace<HlslShaderAsset>(
              asset->GetHandle(), mFileHandler->ReadFile(assetPath));
            break;
          }
        case ASSET_TYPE::TESC_SHADER:
          {
            mRegistry.emplace_or_replace<TessellationControlShaderAsset>(
              asset->GetHandle(), mFileHandler->ReadFile(assetPath));
            break;
          }
        case ASSET_TYPE::TESE_SHADER:
          {
            mRegistry.emplace_or_replace<TessellationEvaluationShaderAsset>(
              asset->GetHandle(), mFileHandler->ReadFile(assetPath));
            break;
          }
        case ASSET_TYPE::VERTEX_SHADER:
          {
            mRegistry.emplace_or_replace<VertexShaderAsset>(
              asset->GetHandle(), mFileHandler->ReadFile(assetPath));
            break;
          }
        case ASSET_TYPE::SCENE:
          {
            mRegistry.emplace_or_replace<SceneAsset>(
              asset->GetHandle(),
              nlohmann::json::parse(mFileHandler->ReadFile(assetPath)));
            break;
          }
        case ASSET_TYPE::UNKNOWN:
          {
            mRegistry.emplace_or_replace<UnknownAsset>(
              asset->GetHandle(), mFileHandler->ReadFile(assetPath));
            break;
          }
      }
    }
  }

  void
  AssetDatabase::Remove(const UUID& uid)
  {
    auto view = mRegistry.view<IDComponent>();
    for (auto entity : view)
    {
      if (view.get<IDComponent>(entity).getId() == uid)
      {
        mRegistry.destroy(entity);
      }
    }
  }
  void
  AssetDatabase::Remove(const std::filesystem::path& path)
  {
    auto view = mRegistry.view<PathComponent>();
    for (auto entity : view)
    {
      if (view.get<PathComponent>(entity).getPath() == path &&
          mRegistry.valid(entity))
      {
        mRegistry.destroy(entity);
      }
    }
  }

  void
  AssetDatabase::Clear()
  {
    mRegistry.clear();
  }

  auto
  AssetDatabase::Import(const std::filesystem::path& assetPath) -> UUID
  {
    std::string           fileName = assetPath.filename().string();
    std::filesystem::path metaDataPath =
      IAssetMetadata::CreateMetadataPath(assetPath);

    // Remove asset if already present
    if (AssetDatabase::Exists(assetPath))
    {
      AssetDatabase::Remove(assetPath);
    }

    auto newId = UUID();
    if (mFileHandler->FileExists(metaDataPath))
    {
      nlohmann::json metaData = mAssetMetadata->GetMetadata(assetPath);
      std::string    guid = metaData["guid"].get<std::string>();
      newId = UUID(guid);
    }
    else
    {
      nlohmann::json metaData;
      metaData["guid"] = newId.toString();
      mAssetMetadata->SetMetadata(assetPath, metaData);
    }

    mLogger->LogInfo(
      Log("Importing asset: " + assetPath.string(), "AssetDatabase"));

    return mAssetReferenceFactory
      ->CreateNew(mRegistry.create(), mRegistry, newId, assetPath, fileName)
      ->GetUID();
  }

  auto
  AssetDatabase::Exists(const UUID& uid) -> bool
  {
    // Retrieve entt::entity with UID component
    auto view = mRegistry.view<IDComponent>();
    return std::ranges::any_of(
      view,
      [view, uid](auto entity)
      { return view.get<IDComponent>(entity).getId() == uid; });
  }

  auto
  AssetDatabase::Exists(const std::filesystem::path& path) -> bool
  {
    // Retrieve entt::entity with UID component
    auto view = mRegistry.view<PathComponent>();
    return std::ranges::any_of(
      view,
      [view, path](auto entity)
      { return view.get<PathComponent>(entity).getPath() == path; });
  }

  auto
  AssetDatabase::GetAssetDirectoryPath() -> std::filesystem::path
  {
    return mAssetDirectoryPath.t;
  }

  auto
  AssetDatabase::Retrieve(const UUID& uid) -> std::unique_ptr<IAssetReference>
  {
    auto view = mRegistry.view<IDComponent, PathComponent>();
    for (auto entity : view)
    {
      if (view.get<IDComponent>(entity).getId() == uid)
      {
        return mAssetReferenceFactory->Create(
          entity,
          mRegistry,
          AssetDatabase::GetAssetType(
            view.get<PathComponent>(entity).getPath().extension().string()));
      }
    }
    return nullptr;
  }

  auto
  AssetDatabase::Retrieve(const std::filesystem::path& path)
    -> std::unique_ptr<IAssetReference>
  {
    auto view = mRegistry.view<PathComponent>();
    for (auto entity : view)
    {
      if (view.get<PathComponent>(entity).getPath() == path)
      {
        return mAssetReferenceFactory->Create(
          entity,
          mRegistry,
          AssetDatabase::GetAssetType(path.extension().string()));
      }
    }
    return nullptr;
  }

  void
  AssetDatabase::Rename(const std::filesystem::path& fromPath,
                        const std::filesystem::path& toPath)
  {
    mAssetMetadata->Rename(fromPath, toPath);
    auto view = mRegistry.view<PathComponent, NameComponent>();
    // auto matView = mRegistry->view<MaterialAsset>();
    for (auto entity : view)
    {
      if (view.get<PathComponent>(entity).getPath() == fromPath)
      {
        mRegistry.remove<PathComponent>(entity);
        mRegistry.remove<NameComponent>(entity);
        mRegistry.emplace<PathComponent>(entity, toPath);
        mRegistry.emplace<NameComponent>(entity, toPath.stem().string());

        // if (matView.contains(entity))
        // {
        //   matView.get<MaterialAsset>(entity).mMaterial->GetProperties()
        //   =
        //     to.stem().string();
        // }
        break;
      }
    }
  }

  void
  AssetDatabase::RenameDirectory(const std::filesystem::path& fromPath,
                                 const std::filesystem::path& toPath)
  {
    auto view = mRegistry.view<PathComponent>();
    for (auto entity : view)
    {
      if (view.get<PathComponent>(entity).getPath().string().find(
            fromPath.string()) != std::string::npos)
      {
        std::filesystem::path newPath = toPath;
        newPath.concat(view.get<PathComponent>(entity).getPath().string().erase(
          0, fromPath.string().length()));
        mRegistry.remove<PathComponent>(entity);
        mRegistry.emplace<PathComponent>(entity, newPath);
      }
    }
  }

  void
  AssetDatabase::AddAssetCallback(const std::string& dir,
                                  const std::string& filename)
  {
    std::filesystem::path path =
      std::filesystem::path(dir) / std::filesystem::path(filename);
    if (!IAssetMetadata::IsMetadataPath(path) &&
        std::filesystem::is_regular_file(path))
    {
      mAssetReimporter->QueueReimport(path);
    }
  }

  void
  AssetDatabase::DeleteAssetCallback(const std::string& dir,
                                     const std::string& filename)
  {
    // std::filesystem::path path =
    //   std::filesystem::path(dir) / std::filesystem::path(filename);
    //  TODO: Mark asset as deleted/missing
  }

  void
  AssetDatabase::ModifyAssetCallback(const std::string& dir,
                                     const std::string& filename)
  {
    std::filesystem::path path =
      std::filesystem::path(dir) / std::filesystem::path(filename);
    if (!IAssetMetadata::IsMetadataPath(path) &&
        std::filesystem::is_regular_file(path))
    {
      mAssetReimporter->QueueReimport(path);
      switch (IAssetDatabase::GetAssetType(path.extension().string()))
      {
        using enum ASSET_TYPE;
        case COMPUTE_SHADER:
        case FRAGMENT_SHADER:
        case GEOMETRY_SHADER:
        case HLSL_SHADER:
        case TESC_SHADER:
        case TESE_SHADER:
        case VERTEX_SHADER:
          {
            if (mShaderAssetMap.contains(path))
            {
              mLogger->LogInfo(
                Log("A shader asset has been updated!", "AssetDatabase"));
              mShaderRecompiler->MarkForRecompilation(mShaderAssetMap[path]);
              // AssetDatabase::AddShaderToRecompilationQueue(path);
            }
            break;
          }
        case ASSET_TYPE::MATERIAL:
          {
            mLogger->LogInfo(
              Log("A material asset has been updated!", "AssetDatabase"));
            // if (AssetDatabase::Exists(path))
            // {
            //   MaterialAsset& mat =
            //     (MaterialAsset&)AssetDatabase::Retrieve(path)->GetAsset();
            //   mShaderRecompiler->MarkForRecompilation(
            //     mat.GetMaterial().GetShader());
            // }
            break;
          }
        case ASSET_TYPE::MODEL:
          {
            // TODO: REIMPORT MODEL FILE
            mLogger->LogInfo(
              Log("A model asset has been updated!", "AssetDatabase"));
            break;
          }
        case ASSET_TYPE::TEXTURE:
          {
            // TODO: REIMPORT TEXTURE
            mLogger->LogInfo(
              Log("A texture asset has been updated!", "AssetDatabase"));
            break;
          }
        case ASSET_TYPE::SCENE:
          {
            // TODO: IF ITS THE CURRENTLY OPEN SCENE, MODAL TO ASK IF IT
            // SHOULD BE RELOADED
            mLogger->LogInfo(
              Log("A scene asset has been updated!", "AssetDatabase"));
            break;
          }
        case ASSET_TYPE::UNKNOWN:
          {
            mLogger->LogInfo(
              Log("An unsupported asset has been updated!", "AssetDatabase"));
            break;
          }
      }
    }
  }

  void
  AssetDatabase::MoveAssetCallback(const std::string& dir,
                                   const std::string& filename,
                                   const std::string& oldFilename)
  {
    std::filesystem::path path =
      std::filesystem::path(dir) / std::filesystem::path(filename);
    // Update PathComponent
    auto view = mRegistry.view<PathComponent>();
    for (auto entity : view)
    {
      if (view.get<PathComponent>(entity).getPath() == path)
      {
        mRegistry.remove<PathComponent>(entity);
        mRegistry.emplace<PathComponent>(entity,
                                         std::filesystem::path(dir) /
                                           std::filesystem::path(oldFilename));
        break;
      }
    }
  }

  auto
  AssetDatabase::GetRegistry() -> entt::registry&
  {
    return mRegistry;
  }

  void
  AssetDatabase::ImportDefaultAssets()
  {
    ImportDefaultShaders();
    ImportDefaultTextures();
    ImportDefaultMaterials();
    ImportDefaultModels();
  }

  void
  AssetDatabase::ImportDefaultShaders()
  {
    std::filesystem::path shaderDir = "data/engine/shaders/";

    std::filesystem::path graphicsApiDir = "";

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL: graphicsApiDir = "opengl"; break;
      case GraphicsApi::Vulkan: graphicsApiDir = "vulkan"; break;
      case GraphicsApi::D3D12: graphicsApiDir = "d3d12"; break;
      case GraphicsApi::Metal: graphicsApiDir = "metal"; break;
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    std::filesystem::path defaultShaderDir =
      shaderDir / "default" / graphicsApiDir;

    for (const auto& directoryEntry :
         std::filesystem::directory_iterator(defaultShaderDir.make_preferred()))
    {
      if (directoryEntry.is_regular_file() &&
          directoryEntry.path().extension() != ".dmeta")
      {
        Import(directoryEntry.path());
      }
    }

    std::filesystem::path errorShaderDir = shaderDir / "error" / graphicsApiDir;

    for (const auto& directoryEntry :
         std::filesystem::directory_iterator(errorShaderDir))
    {
      if (directoryEntry.is_regular_file() &&
          directoryEntry.path().extension() != ".dmeta")
      {
        Import(directoryEntry.path());
      }
    }

    std::filesystem::path gridShaderDir = shaderDir / "grid" / graphicsApiDir;

    for (const auto& directoryEntry :
         std::filesystem::directory_iterator(gridShaderDir))
    {
      if (directoryEntry.is_regular_file() &&
          directoryEntry.path().extension() != ".dmeta")
      {
        Import(directoryEntry.path());
      }
    }

    std::filesystem::path idShaderDir = shaderDir / "id" / graphicsApiDir;

    for (const auto& directoryEntry :
         std::filesystem::directory_iterator(idShaderDir))
    {
      if (directoryEntry.is_regular_file() &&
          directoryEntry.path().extension() != ".dmeta")
      {
        Import(directoryEntry.path());
      }
    }

    std::filesystem::path previewShaderDir =
      shaderDir / "preview" / graphicsApiDir;

    for (const auto& directoryEntry :
         std::filesystem::directory_iterator(previewShaderDir))
    {
      if (directoryEntry.is_regular_file() &&
          directoryEntry.path().extension() != ".dmeta")
      {
        Import(directoryEntry.path());
      }
    }

    std::filesystem::path pbrShaderDir = shaderDir / "pbr" / graphicsApiDir;

    for (const auto& directoryEntry :
         std::filesystem::directory_iterator(pbrShaderDir))
    {
      if (directoryEntry.is_regular_file() &&
          directoryEntry.path().extension() != ".dmeta")
      {
        Import(directoryEntry.path());
      }
    }

    std::filesystem::path fullscreenQuadShaderPath =
      shaderDir / "fullscreen_quad" / graphicsApiDir / "fullscreen_quad.vert";

    Import(fullscreenQuadShaderPath);

    std::filesystem::path agxShaderPath =
      shaderDir / "agx_tonemap" / graphicsApiDir / "agx_tonemap.frag";
    Import(agxShaderPath);
  }

  void
  AssetDatabase::ImportDefaultTextures()
  {
  }

  void
  AssetDatabase::ImportDefaultMaterials()
  {
  }

  void
  AssetDatabase::ImportDefaultModels()
  {
  }
}