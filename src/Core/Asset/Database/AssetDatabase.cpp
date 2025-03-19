#include "AssetDatabase.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "Core/Base.h"
#include "Core/GenericComponents.h"
#include "Core/Rendering/Material/IMaterial.h"
#include "IAssetDatabase.h"
#include <cerrno>
#include <filesystem>

namespace Dwarf
{
  AssetDatabase::AssetDatabase(
    AssetDirectoryPath                       assetDirectoryPath,
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
    : m_AssetDirectoryPath(assetDirectoryPath)
    , m_GraphicsApi(graphicsApi)
    , m_Logger(logger)
    , m_AssetDirectoryListener(assetDirectoryListener)
    , m_AssetMetadata(assetMetadata)
    , m_ModelImporter(modelImporter)
    , m_ShaderRecompiler(shaderRecompiler)
    , m_TextureFactory(textureFactory)
    , m_MaterialFactory(materialFactory)
    , m_MaterialIO(materialIO)
    , m_AssetReimporter(assetReimporter)
    , m_AssetReferenceFactory(assetReferenceFactory)
    , m_FileHandler(fileHandler)
    , m_Registry(entt::registry())
  {
    if (!m_FileHandler->DirectoryExists(m_AssetDirectoryPath.t))
    {
      m_FileHandler->CreateDirectoryAt(m_AssetDirectoryPath.t);
    }

    ImportDefaultAssets();

    m_AssetDirectoryListener->registerAddFileCallback(
      std::bind(&AssetDatabase::AddAssetCallback,
                this,
                std::placeholders::_1,
                std::placeholders::_2));
    m_AssetDirectoryListener->registerDeleteFileCallback(
      std::bind(&AssetDatabase::DeleteAssetCallback,
                this,
                std::placeholders::_1,
                std::placeholders::_2));
    m_AssetDirectoryListener->registerModifyFileCallback(
      std::bind(&AssetDatabase::ModifyAssetCallback,
                this,
                std::placeholders::_1,
                std::placeholders::_2));
    m_AssetDirectoryListener->registerMoveFileCallback(
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

    for (auto& directoryEntry : std::filesystem::directory_iterator(directory))
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
    // RecursiveImport(m_AssetDirectoryPath.t);

    GatherAssetPaths(m_AssetDirectoryPath, materialPaths, otherPaths);

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
            m_Registry.emplace_or_replace<ModelAsset>(
              asset->GetHandle(), m_ModelImporter->Import(assetPath));
            break;
          }
        case ASSET_TYPE::TEXTURE:
          {
            // m_Registry.replace<TextureAsset>(
            //   asset->GetHandle(),
            //   m_TextureFactory->FromPath(assetPath),
            //   m_TextureFactory->GetPlaceholderTexture());

            m_Registry.get<TextureAsset>(asset->GetHandle())
              .SetTexture(nullptr);
            break;
          }
        case ASSET_TYPE::MATERIAL:
          {
            m_Registry.emplace_or_replace<MaterialAsset>(
              asset->GetHandle(), m_MaterialIO->LoadMaterial(assetPath));
            break;
          }
        case ASSET_TYPE::COMPUTE_SHADER:
          {
            m_Registry.emplace_or_replace<ComputeShaderAsset>(
              asset->GetHandle(), m_FileHandler->ReadFile(assetPath));
            break;
          }
        case ASSET_TYPE::FRAGMENT_SHADER:
          {
            m_Registry.emplace_or_replace<FragmentShaderAsset>(
              asset->GetHandle(), m_FileHandler->ReadFile(assetPath));
            break;
          }
        case ASSET_TYPE::GEOMETRY_SHADER:
          {
            m_Registry.emplace_or_replace<GeometryShaderAsset>(
              asset->GetHandle(), m_FileHandler->ReadFile(assetPath));
            break;
          }
        case ASSET_TYPE::HLSL_SHADER:
          {
            m_Registry.emplace_or_replace<HlslShaderAsset>(
              asset->GetHandle(), m_FileHandler->ReadFile(assetPath));
            break;
          }
        case ASSET_TYPE::TESC_SHADER:
          {
            m_Registry.emplace_or_replace<TessellationControlShaderAsset>(
              asset->GetHandle(), m_FileHandler->ReadFile(assetPath));
            break;
          }
        case ASSET_TYPE::TESE_SHADER:
          {
            m_Registry.emplace_or_replace<TessellationEvaluationShaderAsset>(
              asset->GetHandle(), m_FileHandler->ReadFile(assetPath));
            break;
          }
        case ASSET_TYPE::VERTEX_SHADER:
          {
            m_Registry.emplace_or_replace<VertexShaderAsset>(
              asset->GetHandle(), m_FileHandler->ReadFile(assetPath));
            break;
          }
        case ASSET_TYPE::SCENE:
          {
            m_Registry.emplace_or_replace<SceneAsset>(
              asset->GetHandle(),
              nlohmann::json::parse(m_FileHandler->ReadFile(assetPath)));
            break;
          }
        case ASSET_TYPE::UNKNOWN:
          {
            m_Registry.emplace_or_replace<UnknownAsset>(
              asset->GetHandle(), m_FileHandler->ReadFile(assetPath));
            break;
          }
      }
    }
  }

  void
  AssetDatabase::Remove(const UUID& uid)
  {
    auto view = m_Registry.view<IDComponent>();
    for (auto entity : view)
    {
      if (view.get<IDComponent>(entity).getId() == uid)
      {
        m_Registry.destroy(entity);
      }
    }
  }
  void
  AssetDatabase::Remove(const std::filesystem::path& path)
  {
    auto view = m_Registry.view<PathComponent>();
    for (auto entity : view)
    {
      if (view.get<PathComponent>(entity).getPath() == path &&
          m_Registry.valid(entity))
      {
        m_Registry.destroy(entity);
      }
    }
  }

  void
  AssetDatabase::Clear()
  {
    m_Registry.clear();
  }

  UUID
  AssetDatabase::Import(const std::filesystem::path& assetPath)
  {
    std::string           fileName = assetPath.filename().string();
    std::filesystem::path metaDataPath =
      IAssetMetadata::GetMetadataPath(assetPath);

    // Remove asset if already present
    if (AssetDatabase::Exists(assetPath))
    {
      AssetDatabase::Remove(assetPath);
    }

    auto id = UUID();
    if (m_FileHandler->FileExists(metaDataPath))
    {
      nlohmann::json metaData = m_AssetMetadata->GetMetadata(assetPath);
      std::string    guid = metaData["guid"].get<std::string>();
      id = UUID(guid);
    }
    else
    {
      nlohmann::json metaData;
      metaData["guid"] = id.toString();
      m_AssetMetadata->SetMetadata(assetPath, metaData);
    }

    m_Logger->LogInfo(
      Log("Importing asset: " + assetPath.string(), "AssetDatabase"));

    return m_AssetReferenceFactory
      ->CreateNew(m_Registry.create(), m_Registry, id, assetPath, fileName)
      ->GetUID();
  }

  bool
  AssetDatabase::Exists(const UUID& uid)
  {
    // Retrieve entt::entity with UID component
    auto view = m_Registry.view<IDComponent>();
    for (auto entity : view)
    {
      if (view.get<IDComponent>(entity).getId() == uid)
      {
        return true;
      }
    }
    return false;
  }

  bool
  AssetDatabase::Exists(const std::filesystem::path& path)
  {
    // Retrieve entt::entity with UID component
    auto view = m_Registry.view<PathComponent>();
    for (auto entity : view)
    {
      if (view.get<PathComponent>(entity).getPath() == path)
      {
        return true;
      }
    }
    return false;
  }

  std::filesystem::path
  AssetDatabase::GetAssetDirectoryPath()
  {
    return m_AssetDirectoryPath.t;
  }

  std::unique_ptr<IAssetReference>
  AssetDatabase::Retrieve(const UUID& uid)
  {
    auto view = m_Registry.view<IDComponent, PathComponent>();
    for (auto entity : view)
    {
      if (view.get<IDComponent>(entity).getId() == uid)
      {
        return m_AssetReferenceFactory->Create(
          entity,
          m_Registry,
          AssetDatabase::GetAssetType(
            view.get<PathComponent>(entity).getPath().extension().string()));
      }
    }
    return nullptr;
  }

  std::unique_ptr<IAssetReference>
  AssetDatabase::Retrieve(const std::filesystem::path& path)
  {
    auto view = m_Registry.view<PathComponent>();
    for (auto entity : view)
    {
      if (view.get<PathComponent>(entity).getPath() == path)
      {
        return m_AssetReferenceFactory->Create(
          entity,
          m_Registry,
          AssetDatabase::GetAssetType(path.extension().string()));
      }
    }
    return nullptr;
  }

  void
  AssetDatabase::Rename(const std::filesystem::path& from,
                        const std::filesystem::path& to)
  {
    m_AssetMetadata->Rename(from, to);
    auto view = m_Registry.view<PathComponent, NameComponent>();
    // auto matView = m_Registry->view<MaterialAsset>();
    for (auto entity : view)
    {
      if (view.get<PathComponent>(entity).getPath() == from)
      {
        m_Registry.remove<PathComponent>(entity);
        m_Registry.remove<NameComponent>(entity);
        m_Registry.emplace<PathComponent>(entity, to);
        m_Registry.emplace<NameComponent>(entity, to.stem().string());

        // if (matView.contains(entity))
        // {
        //   matView.get<MaterialAsset>(entity).m_Material->GetProperties()
        //   =
        //     to.stem().string();
        // }
        break;
      }
    }
  }

  void
  AssetDatabase::RenameDirectory(const std::filesystem::path& from,
                                 const std::filesystem::path& to)
  {
    auto view = m_Registry.view<PathComponent>();
    for (auto entity : view)
    {
      if (view.get<PathComponent>(entity).getPath().string().find(
            from.string()) != std::string::npos)
      {
        std::filesystem::path newPath = to;
        newPath.concat(view.get<PathComponent>(entity).getPath().string().erase(
          0, from.string().length()));
        m_Registry.remove<PathComponent>(entity);
        m_Registry.emplace<PathComponent>(entity, newPath);
      }
    }
  }

  void
  AssetDatabase::AddAssetCallback(const std::string& dir,
                                  const std::string& filename)
  {
    std::filesystem::path path =
      std::filesystem::path(dir) / std::filesystem::path(filename);
    if (!m_AssetMetadata->IsMetadataPath(path) &&
        std::filesystem::is_regular_file(path))
    {
      m_AssetReimporter->QueueReimport(path);
      // Import(path);
    }
  }

  void
  AssetDatabase::DeleteAssetCallback(const std::string& dir,
                                     const std::string& filename)
  {
    std::filesystem::path path =
      std::filesystem::path(dir) / std::filesystem::path(filename);
    // TODO: Mark asset as deleted/missing
  }

  void
  AssetDatabase::ModifyAssetCallback(const std::string& dir,
                                     const std::string& filename)
  {
    std::filesystem::path path =
      std::filesystem::path(dir) / std::filesystem::path(filename);
    if (!m_AssetMetadata->IsMetadataPath(path) &&
        std::filesystem::is_regular_file(path))
    {
      m_AssetReimporter->QueueReimport(path);
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
            if (m_ShaderAssetMap.contains(path))
            {
              m_Logger->LogInfo(
                Log("A shader asset has been updated!", "AssetDatabase"));
              m_ShaderRecompiler->MarkForRecompilation(m_ShaderAssetMap[path]);
              // AssetDatabase::AddShaderToRecompilationQueue(path);
            }
            break;
          }
        case ASSET_TYPE::MATERIAL:
          {
            m_Logger->LogInfo(
              Log("A material asset has been updated!", "AssetDatabase"));
            // if (AssetDatabase::Exists(path))
            // {
            //   MaterialAsset& mat =
            //     (MaterialAsset&)AssetDatabase::Retrieve(path)->GetAsset();
            //   m_ShaderRecompiler->MarkForRecompilation(
            //     mat.GetMaterial().GetShader());
            // }
            break;
          }
        case ASSET_TYPE::MODEL:
          {
            // TODO: REIMPORT MODEL FILE
            m_Logger->LogInfo(
              Log("A model asset has been updated!", "AssetDatabase"));
            break;
          }
        case ASSET_TYPE::TEXTURE:
          {
            // TODO: REIMPORT TEXTURE
            m_Logger->LogInfo(
              Log("A texture asset has been updated!", "AssetDatabase"));
            break;
          }
        case ASSET_TYPE::SCENE:
          {
            // TODO: IF ITS THE CURRENTLY OPEN SCENE, MODAL TO ASK IF IT
            // SHOULD BE RELOADED
            m_Logger->LogInfo(
              Log("A scene asset has been updated!", "AssetDatabase"));
            break;
          }
        case ASSET_TYPE::UNKNOWN:
          {
            m_Logger->LogInfo(
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
    auto view = m_Registry.view<PathComponent>();
    for (auto entity : view)
    {
      if (view.get<PathComponent>(entity).getPath() == path)
      {
        m_Registry.remove<PathComponent>(entity);
        m_Registry.emplace<PathComponent>(entity,
                                          std::filesystem::path(dir) /
                                            std::filesystem::path(oldFilename));
        break;
      }
    }
  }

  entt::registry&
  AssetDatabase::GetRegistry()
  {
    return m_Registry;
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

    switch (m_GraphicsApi)
    {
      case GraphicsApi::OpenGL: graphicsApiDir = "opengl"; break;
      case GraphicsApi::Vulkan: graphicsApiDir = "vulkan"; break;
      case GraphicsApi::D3D12: graphicsApiDir = "d3d12"; break;
      case GraphicsApi::Metal: graphicsApiDir = "metal"; break;
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    std::filesystem::path defaultShaderDir =
      shaderDir / "default" / graphicsApiDir;

    for (auto& directoryEntry :
         std::filesystem::directory_iterator(defaultShaderDir.make_preferred()))
    {
      if (directoryEntry.is_regular_file() &&
          directoryEntry.path().extension() != ".dmeta")
      {
        Import(directoryEntry.path());
      }
    }

    std::filesystem::path errorShaderDir = shaderDir / "error" / graphicsApiDir;

    for (auto& directoryEntry :
         std::filesystem::directory_iterator(errorShaderDir))
    {
      if (directoryEntry.is_regular_file() &&
          directoryEntry.path().extension() != ".dmeta")
      {
        Import(directoryEntry.path());
      }
    }

    std::filesystem::path gridShaderDir = shaderDir / "grid" / graphicsApiDir;

    for (auto& directoryEntry :
         std::filesystem::directory_iterator(gridShaderDir))
    {
      if (directoryEntry.is_regular_file() &&
          directoryEntry.path().extension() != ".dmeta")
      {
        Import(directoryEntry.path());
      }
    }

    std::filesystem::path idShaderDir = shaderDir / "id" / graphicsApiDir;

    for (auto& directoryEntry :
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

    for (auto& directoryEntry :
         std::filesystem::directory_iterator(previewShaderDir))
    {
      if (directoryEntry.is_regular_file() &&
          directoryEntry.path().extension() != ".dmeta")
      {
        Import(directoryEntry.path());
      }
    }

    std::filesystem::path pbrShaderDir = shaderDir / "pbr" / graphicsApiDir;

    for (auto& directoryEntry :
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