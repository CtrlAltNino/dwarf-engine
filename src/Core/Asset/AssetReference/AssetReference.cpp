#include "AssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Texture/TextureWorker/ITextureLoadingWorker.h"
#include "Core/GenericComponents.h"

namespace Dwarf
{
  // Assuming already emplaced components
  AssetReference::AssetReference(
    entt::entity                           assetHandle,
    entt::registry&                        registry,
    ASSET_TYPE                             type,
    std::shared_ptr<ITextureLoadingWorker> textureLoadingWorker,
    std::shared_ptr<IDwarfLogger>          logger)
    : m_AssetHandle(assetHandle)
    , m_Registry(registry)
    , m_Type(type)
    , m_Logger(logger)
    , m_TextureLoadingWorker(textureLoadingWorker)
  {
  }

  // Assuming no components have been emplaced
  AssetReference::AssetReference(
    entt::entity                           assetHandle,
    entt::registry&                        registry,
    UUID                                   uid,
    std::filesystem::path                  path,
    std::string                            name,
    std::shared_ptr<IDwarfLogger>          logger,
    std::shared_ptr<IModelImporter>        modelImporter,
    std::shared_ptr<ITextureFactory>       textureFactory,
    std::shared_ptr<IMaterialIO>           materialIO,
    std::shared_ptr<IFileHandler>          fileHandler,
    std::shared_ptr<ITextureLoadingWorker> textureLoadingWorker)
    : m_AssetHandle(assetHandle)
    , m_Registry(registry)
    , m_Type(IAssetDatabase::GetAssetType(path.extension().string()))
    , m_Logger(logger)
    , m_TextureLoadingWorker(textureLoadingWorker)
  {
    m_Registry.emplace<IDComponent>(m_AssetHandle, uid);
    m_Registry.emplace<PathComponent>(m_AssetHandle, path);
    m_Registry.emplace<NameComponent>(m_AssetHandle, name);

    switch (m_Type)
    {
      case ASSET_TYPE::TEXTURE:
        {
          m_Registry.emplace<TextureAsset>(
            m_AssetHandle, nullptr, textureFactory->GetPlaceholderTexture());
          break;
        }
      case ASSET_TYPE::MODEL:
        {
          ModelAsset& modelAsset = m_Registry.emplace<ModelAsset>(
            m_AssetHandle, std::move(modelImporter->Import(path)));
        }
        break;
      case ASSET_TYPE::MATERIAL:
        m_Registry.emplace<MaterialAsset>(
          m_AssetHandle, std::move(materialIO->LoadMaterial(path)));
        break;
      case ASSET_TYPE::UNKNOWN:
        m_Registry.emplace<UnknownAsset>(m_AssetHandle,
                                         fileHandler->ReadFile(path));
        break;
      case ASSET_TYPE::SCENE:
        if (fileHandler->FileExists(path))
        {
          m_Registry.emplace<SceneAsset>(
            m_AssetHandle, nlohmann::json::parse(fileHandler->ReadFile(path)));
        }
        break;
      case ASSET_TYPE::VERTEX_SHADER:
        m_Registry.emplace<VertexShaderAsset>(m_AssetHandle,
                                              fileHandler->ReadFile(path));
        break;
      case ASSET_TYPE::TESC_SHADER:
        m_Registry.emplace<TessellationControlShaderAsset>(
          m_AssetHandle, fileHandler->ReadFile(path));
        break;
      case ASSET_TYPE::TESE_SHADER:
        m_Registry.emplace<TessellationEvaluationShaderAsset>(
          m_AssetHandle, fileHandler->ReadFile(path));
        break;
      case ASSET_TYPE::GEOMETRY_SHADER:
        m_Registry.emplace<GeometryShaderAsset>(m_AssetHandle,
                                                fileHandler->ReadFile(path));
        break;
      case ASSET_TYPE::FRAGMENT_SHADER:
        m_Registry.emplace<FragmentShaderAsset>(m_AssetHandle,
                                                fileHandler->ReadFile(path));
        break;
      case ASSET_TYPE::COMPUTE_SHADER:
        m_Registry.emplace<ComputeShaderAsset>(m_AssetHandle,
                                               fileHandler->ReadFile(path));
        break;
      case ASSET_TYPE::HLSL_SHADER:
        m_Registry.emplace<HlslShaderAsset>(m_AssetHandle,
                                            fileHandler->ReadFile(path));
        break;
    }
  }

  /// @brief Retrieves the handle of the asset entity.
  /// @return The handle.
  entt::entity
  AssetReference::GetHandle() const
  {
    return m_AssetHandle;
  }

  /// @brief Returns the UID of the asset.
  /// @return The UID.
  const UUID&
  AssetReference::GetUID() const
  {
    return m_Registry.get<IDComponent>(m_AssetHandle).GetID();
  }

  const std::filesystem::path&
  AssetReference::GetPath() const
  {
    return m_Registry.get<PathComponent>(m_AssetHandle).GetPath();
  }

  /// @brief Retrieves the asset component of the asset, containing the actual
  /// payload.
  /// @return The asset component.
  IAssetComponent&
  AssetReference::GetAsset()
  {
    switch (m_Type)
    {
      case ASSET_TYPE::TEXTURE:
        {
          TextureAsset& asset = m_Registry.get<TextureAsset>(m_AssetHandle);
          std::filesystem::path path =
            m_Registry.get<PathComponent>(m_AssetHandle).GetPath();

          if (!asset.IsLoaded() && !m_TextureLoadingWorker->IsRequested(path))
          {
            m_TextureLoadingWorker->RequestTextureLoad({ &asset, path });
          }

          return asset;
        }
      case ASSET_TYPE::MODEL: return m_Registry.get<ModelAsset>(m_AssetHandle);
      case ASSET_TYPE::MATERIAL:
        return m_Registry.get<MaterialAsset>(m_AssetHandle);
      case ASSET_TYPE::UNKNOWN:
        return m_Registry.get<UnknownAsset>(m_AssetHandle);
      case ASSET_TYPE::SCENE: return m_Registry.get<SceneAsset>(m_AssetHandle);
      case ASSET_TYPE::VERTEX_SHADER:
        return m_Registry.get<VertexShaderAsset>(m_AssetHandle);
      case ASSET_TYPE::TESC_SHADER:
        return m_Registry.get<TessellationControlShaderAsset>(m_AssetHandle);
      case ASSET_TYPE::TESE_SHADER:
        return m_Registry.get<TessellationEvaluationShaderAsset>(m_AssetHandle);
      case ASSET_TYPE::GEOMETRY_SHADER:
        return m_Registry.get<GeometryShaderAsset>(m_AssetHandle);
      case ASSET_TYPE::FRAGMENT_SHADER:
        return m_Registry.get<FragmentShaderAsset>(m_AssetHandle);
      case ASSET_TYPE::COMPUTE_SHADER:
        return m_Registry.get<ComputeShaderAsset>(m_AssetHandle);
      case ASSET_TYPE::HLSL_SHADER:
        return m_Registry.get<HlslShaderAsset>(m_AssetHandle);
      default: return m_Registry.get<UnknownAsset>(m_AssetHandle);
    }
  }

  ASSET_TYPE
  AssetReference::GetType() const
  {
    return m_Type;
  }
}