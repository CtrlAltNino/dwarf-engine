#include "AssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/GenericComponents.h"

namespace Dwarf
{
  // Assuming already emplaced components
  AssetReference::AssetReference(
    entt::entity                     assetHandle,
    entt::registry&                  registry,
    ASSET_TYPE                       type,
    std::shared_ptr<IModelImporter>  modelImporter,
    std::shared_ptr<ITextureFactory> textureFactory,
    std::shared_ptr<IMaterialIO>     materialIO)
    : m_AssetHandle(assetHandle)
    , m_Registry(registry)
    , m_Type(type)
    , m_ModelImporter(modelImporter)
    , m_TextureFactory(textureFactory)
    , m_MaterialIO(materialIO)
  {
  }

  // Assuming no components have been emplaced
  AssetReference::AssetReference(
    entt::entity                     assetHandle,
    entt::registry&                  registry,
    UUID                             uid,
    std::filesystem::path            path,
    std::string                      name,
    std::shared_ptr<IModelImporter>  modelImporter,
    std::shared_ptr<ITextureFactory> textureFactory,
    std::shared_ptr<IMaterialIO>     materialIO)
    : m_AssetHandle(assetHandle)
    , m_Registry(registry)
    , m_Type(IAssetDatabase::GetAssetType(path.extension().string()))
    , m_ModelImporter(modelImporter)
    , m_TextureFactory(textureFactory)
    , m_MaterialIO(materialIO)
  {
    m_Registry.emplace<IDComponent>(m_AssetHandle, uid);
    m_Registry.emplace<PathComponent>(m_AssetHandle, path);
    m_Registry.emplace<NameComponent>(m_AssetHandle, name);

    switch (m_Type)
    {
      case ASSET_TYPE::TEXTURE:
        m_Registry.emplace<TextureAsset>(m_AssetHandle,
                                         m_TextureFactory->FromPath(path));
        break;
      case ASSET_TYPE::MODEL:
        m_Registry.emplace<ModelAsset>(m_AssetHandle,
                                       m_ModelImporter->Import(path));
        break;
      case ASSET_TYPE::MATERIAL:
        m_Registry.emplace<MaterialAsset>(m_AssetHandle,
                                          m_MaterialIO->LoadMaterial(path));
        break;
      case ASSET_TYPE::UNKNOWN:
        m_Registry.emplace<UnknownAsset>(m_AssetHandle, path);
        break;
      case ASSET_TYPE::SCENE:
        m_Registry.emplace<SceneAsset>(m_AssetHandle, path);
        break;
      case ASSET_TYPE::VERTEX_SHADER:
        m_Registry.emplace<VertexShaderAsset>(m_AssetHandle, path);
        break;
      case ASSET_TYPE::TESC_SHADER:
        m_Registry.emplace<TessellationControlShaderAsset>(m_AssetHandle, path);
        break;
      case ASSET_TYPE::TESE_SHADER:
        m_Registry.emplace<TessellationEvaluationShaderAsset>(m_AssetHandle,
                                                              path);
        break;
      case ASSET_TYPE::GEOMETRY_SHADER:
        m_Registry.emplace<GeometryShaderAsset>(m_AssetHandle, path);
        break;
      case ASSET_TYPE::FRAGMENT_SHADER:
        m_Registry.emplace<FragmentShaderAsset>(m_AssetHandle, path);
        break;
      case ASSET_TYPE::COMPUTE_SHADER:
        m_Registry.emplace<ComputeShaderAsset>(m_AssetHandle, path);
        break;
      case ASSET_TYPE::HLSL_SHADER:
        m_Registry.emplace<HlslShaderAsset>(m_AssetHandle, path);
        break;
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
        return m_Registry.get<TextureAsset>(m_AssetHandle);
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
    }
  }

  ASSET_TYPE
  AssetReference::GetType() const
  {
    return m_Type;
  }
}