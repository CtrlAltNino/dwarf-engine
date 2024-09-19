#include "AssetDatabase.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "Core/GenericComponents.h"
#include "Core/Rendering/Material/IMaterial.h"
#include "Utilities/FileHandler.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Database/AssetComponents.h"
#include <filesystem>

namespace Dwarf
{
  AssetDatabase::AssetDatabase(
    AssetDirectoryPath                       assetDirectoryPath,
    std::shared_ptr<IDwarfLogger>            logger,
    std::shared_ptr<IAssetDirectoryListener> assetDirectoryListener,
    std::shared_ptr<IAssetMetadata>          assetMetadata,
    std::shared_ptr<IModelImporter>          modelImporter,
    std::shared_ptr<IShaderRecompiler>       shaderRecompiler,
    std::shared_ptr<ITextureFactory>         textureFactory,
    std::shared_ptr<IMaterialFactory>        materialFactory,
    std::shared_ptr<IMaterialIO>             materialIO,
    std::shared_ptr<IAssetReimporter>        assetReimporter,
    std::shared_ptr<IAssetReferenceFactory>  assetReferenceFactory)
    : m_AssetDirectoryPath(assetDirectoryPath)
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
    , m_Registry(entt::registry())
  {
    if (!FileHandler::DirectoyExists(m_AssetDirectoryPath.t))
    {
      FileHandler::CreateDirectoryAt(m_AssetDirectoryPath.t);
    }

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
  AssetDatabase::RecursiveImport(std::filesystem::path const& directory)
  {
    for (auto& directoryEntry : std::filesystem::directory_iterator(directory))
    {
      if (directoryEntry.is_directory())
      {
        RecursiveImport(directoryEntry.path());
      }
      else if (directoryEntry.is_regular_file() &&
               directoryEntry.path().has_extension() &&
               !IAssetMetadata::IsMetadataPath(directoryEntry))
      {
        Import(directoryEntry.path());
      }
    }
  }

  void
  AssetDatabase::ReimportAll()
  {
    Clear();
    RecursiveImport(m_AssetDirectoryPath.t);
  }

  UUID
  AssetDatabase::Reimport(std::filesystem::path const& assetPath)
  {
    return AssetDatabase::Import(assetPath);
  }

  void
  AssetDatabase::Remove(const UUID& uid)
  {
    auto view = m_Registry.view<IDComponent>();
    for (auto entity : view)
    {
      if (view.get<IDComponent>(entity).GetID() == uid)
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
      if (view.get<PathComponent>(entity).GetPath() == path &&
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
    if (FileHandler::FileExists(metaDataPath))
    {
      nlohmann::json metaData = m_AssetMetadata->GetMetadata(assetPath);
      std::string    guid = metaData["guid"].get<std::string>();
      id = UUID(guid);
    }
    else
    {
      nlohmann::json metaData;
      metaData["guid"] = id.ToString();
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
      if (view.get<IDComponent>(entity).GetID() == uid)
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
      if (view.get<PathComponent>(entity).GetPath() == path)
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
      if (view.get<IDComponent>(entity).GetID() == uid)
      {
        return m_AssetReferenceFactory->Create(
          entity,
          m_Registry,
          AssetDatabase::GetAssetType(
            view.get<PathComponent>(entity).GetPath().extension().string()));
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
      if (view.get<PathComponent>(entity).GetPath() == path)
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
      if (view.get<PathComponent>(entity).GetPath() == from)
      {
        m_Registry.remove<PathComponent>(entity);
        m_Registry.remove<NameComponent>(entity);
        m_Registry.emplace<PathComponent>(entity, to);
        m_Registry.emplace<NameComponent>(entity, to.stem().string());

        // if (matView.contains(entity))
        // {
        //   matView.get<MaterialAsset>(entity).m_Material->GetProperties() =
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
      if (view.get<PathComponent>(entity).GetPath().string().find(
            from.string()) != std::string::npos)
      {
        std::filesystem::path newPath = to;
        newPath.concat(view.get<PathComponent>(entity).GetPath().string().erase(
          0, from.string().length()));
        m_Registry.remove<PathComponent>(entity);
        m_Registry.emplace<PathComponent>(entity, newPath);
      }
    }
  }

  // void
  // AssetDatabase::CreateNewMaterialAsset()
  // {
  //   CreateNewMaterialAsset(m_AssetDirectoryPath);
  // }

  // void
  // AssetDatabase::CreateNewMaterialAsset(std::filesystem::path const& path)
  // {
  //   auto newMat = Material("New Material");
  //   newMat.GenerateShaderParameters();
  //   std::filesystem::path newMatPath = path / "New Material.dmat";

  //   while (FileHandler::CheckIfFileExists(newMatPath))
  //   {
  //     std::filesystem::path fileNameWithoutExtension =
  //       newMatPath.filename().replace_extension("");
  //     std::string lastPart = fileNameWithoutExtension.string();

  //     size_t      pos = 0;
  //     std::string token;
  //     while ((pos = lastPart.find(" ")) != std::string::npos)
  //     {
  //       lastPart.erase(0, pos + 1);
  //     }

  //     bool isNumber = true;

  //     for (char c : lastPart)
  //     {
  //       if (!std::isdigit(c))
  //       {
  //         isNumber = false;
  //       }
  //     }

  //     if (isNumber)
  //     {
  //       int val = stoi(lastPart);
  //       val++;

  //       newMatPath.replace_filename(
  //         fileNameWithoutExtension.string().substr(
  //           0, fileNameWithoutExtension.string().length() -
  //           lastPart.length()) +
  //         std::to_string(val) + std::string(".dmat"));
  //     }
  //     else
  //     {
  //       newMatPath.replace_filename(fileNameWithoutExtension.string() +
  //                                   std::string(" 2") +
  //                                   std::string(".dmat"));
  //     }
  //   }
  //   m_MaterialSerializer->Serialize(newMat, newMatPath);
  // }

  // std::filesystem::path
  // AssetDatabase::GetAssetDirectoryPath()
  // {
  //   return m_AssetDirectoryPath;
  // }

  // void
  // AssetDatabase::RecompileShaders()
  // {
  //   for (std::shared_ptr<IShader> shader : m_ShaderRecompilationStack)
  //   {
  //     shader->Compile();
  //   }
  //   m_ShaderRecompilationStack.clear();
  // }

  // void
  // AssetDatabase::AddShaderWatch(std::filesystem::path const& shaderAssetPath,
  //                               std::shared_ptr<IShader>     shader)
  // {
  //   m_ShaderAssetMap[shaderAssetPath] = shader;
  // }

  // void
  // AssetDatabase::RemoveShaderWatch(std::filesystem::path const&
  // shaderAssetPath)
  // {
  //   m_ShaderAssetMap.erase(shaderAssetPath);
  // }

  // void
  // AssetDatabase::AddShaderToRecompilationQueue(
  //   std::filesystem::path const& path)
  // {
  //   m_ShaderRecompilationStack.push_back(m_ShaderAssetMap[path]);
  // }

  // void
  // AssetDatabase::AddShaderToRecompilationQueue(std::shared_ptr<IShader>
  // shader)
  // {
  //   m_ShaderRecompilationStack.push_back(shader);
  // }

  /*std::any
  AssetDatabase::CreateAssetReference(std::type_index type, entt::entity entity)
  {
    if (type == typeid(ModelAsset))
    {
      return m_AssetReferenceFactory->Create<ModelAsset>(entity, m_Registry);
    }
    else if (type == typeid(MaterialAsset))
    {
      return m_AssetReferenceFactory->Create<MaterialAsset>(entity, m_Registry);
    }
    else if (type == typeid(TextureAsset))
    {
      return m_AssetReferenceFactory->Create<TextureAsset>(entity, m_Registry);
    }
    else if (type == typeid(SceneAsset))
    {
      return m_AssetReferenceFactory->Create<SceneAsset>(entity, m_Registry);
    }
    else if (type == typeid(VertexShaderAsset))
    {
      return m_AssetReferenceFactory->Create<VertexShaderAsset>(entity,
                                                                m_Registry);
    }
    else if (type == typeid(TessellationControlShaderAsset))
    {
      return m_AssetReferenceFactory->Create<TessellationControlShaderAsset>(
        entity, m_Registry);
    }
    else if (type == typeid(TessellationEvaluationShaderAsset))
    {
      return m_AssetReferenceFactory->Create<TessellationEvaluationShaderAsset>(
        entity, m_Registry);
    }
    else if (type == typeid(GeometryShaderAsset))
    {
      return m_AssetReferenceFactory->Create<GeometryShaderAsset>(entity,
                                                                  m_Registry);
    }
    else if (type == typeid(FragmentShaderAsset))
    {
      return m_AssetReferenceFactory->Create<FragmentShaderAsset>(entity,
                                                                  m_Registry);
    }
    else if (type == typeid(ComputeShaderAsset))
    {
      return m_AssetReferenceFactory->Create<ComputeShaderAsset>(entity,
                                                                 m_Registry);
    }
    else if (type == typeid(HlslShaderAsset))
    {
      return m_AssetReferenceFactory->Create<HlslShaderAsset>(entity,
                                                              m_Registry);
    }
    else
    {
      return m_AssetReferenceFactory->Create<UnknownAsset>(entity, m_Registry);
    }
  }*/

  void
  AssetDatabase::AddAssetCallback(const std::string& dir,
                                  const std::string& filename)
  {
    std::filesystem::path path =
      std::filesystem::path(dir) / std::filesystem::path(filename);
    if (!m_AssetMetadata->IsMetadataPath(path))
    {
      m_AssetReimporter->QueueReimport(path);
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
    if (!m_AssetMetadata->IsMetadataPath(path))
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
              std::cout << "A shader asset has been updated!" << std::endl;
              m_ShaderRecompiler->MarkForRecompilation(m_ShaderAssetMap[path]);
              // AssetDatabase::AddShaderToRecompilationQueue(path);
            }
            break;
          }
        case ASSET_TYPE::MATERIAL:
          {
            std::cout << "A material asset has been updated!" << std::endl;
            MaterialAsset& mat =
              (MaterialAsset&)AssetDatabase::Retrieve(path)->GetAsset();
            m_ShaderRecompiler->MarkForRecompilation(
              mat.GetMaterial().GetShader());
            break;
          }
        case ASSET_TYPE::MODEL:
          {
            // TODO: REIMPORT MODEL FILE
            std::cout << "A model asset has been updated!" << std::endl;
            break;
          }
        case ASSET_TYPE::TEXTURE:
          {
            // TODO: REIMPORT TEXTURE
            std::cout << "A texture asset has been updated!" << std::endl;
            break;
          }
        case ASSET_TYPE::SCENE:
          {
            // TODO: IF ITS THE CURRENTLY OPEN SCENE, MODAL TO ASK IF IT
            // SHOULD BE RELOADED
            std::cout << "A scene asset has been updated!" << std::endl;
            break;
          }
        case ASSET_TYPE::UNKNOWN:
          {
            std::cout << "An unsupported asset has been updated!" << std::endl;
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
      if (view.get<PathComponent>(entity).GetPath() == path)
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
}