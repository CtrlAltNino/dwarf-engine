#include "AssetReference.h"

#include <utility>

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
    std::shared_ptr<IDwarfLogger>          logger,
    std::shared_ptr<ITextureLoadingWorker> textureLoadingWorker)
    : mAssetHandle(assetHandle)
    , mRegistry(registry)
    , mType(type)
    , mLogger(std::move(logger))
    , mTextureLoadingWorker(std::move(textureLoadingWorker))
  {
  }

  // Assuming no components have been emplaced
  AssetReference::AssetReference(
    entt::entity                            assetHandle,
    entt::registry&                         registry,
    UUID                                    uid,
    std::filesystem::path                   path,
    std::string                             name,
    std::shared_ptr<IDwarfLogger>           logger,
    std::shared_ptr<ITextureLoadingWorker>  textureLoadingWorker,
    const std::shared_ptr<IModelImporter>&  modelImporter,
    const std::shared_ptr<ITextureFactory>& textureFactory,
    const std::shared_ptr<IMaterialIO>&     materialIO,
    const std::shared_ptr<IFileHandler>&    fileHandler)
    : mAssetHandle(assetHandle)
    , mRegistry(registry)
    , mType(IAssetDatabase::GetAssetType(path.extension().string()))
    , mLogger(std::move(logger))
    , mTextureLoadingWorker(std::move(textureLoadingWorker))
  {
    mRegistry.get().emplace<IDComponent>(mAssetHandle, uid);
    mRegistry.get().emplace<PathComponent>(mAssetHandle, path);
    mRegistry.get().emplace<NameComponent>(mAssetHandle, name);

    switch (mType)
    {
      case ASSET_TYPE::TEXTURE:
        {
          mRegistry.get().emplace<TextureAsset>(
            mAssetHandle, nullptr, textureFactory->GetPlaceholderTexture());
          break;
        }
      case ASSET_TYPE::MODEL:
        {
          ModelAsset& modelAsset = mRegistry.get().emplace<ModelAsset>(
            mAssetHandle, std::move(modelImporter->Import(path)));
        }
        break;
      case ASSET_TYPE::MATERIAL:
        mRegistry.get().emplace<MaterialAsset>(
          mAssetHandle, std::move(materialIO->LoadMaterial(path)));
        break;
      case ASSET_TYPE::UNKNOWN:
        mRegistry.get().emplace<UnknownAsset>(mAssetHandle,
                                              fileHandler->ReadFile(path));
        break;
      case ASSET_TYPE::SCENE:
        if (fileHandler->FileExists(path))
        {
          mRegistry.get().emplace<SceneAsset>(
            mAssetHandle, nlohmann::json::parse(fileHandler->ReadFile(path)));
        }
        break;
      case ASSET_TYPE::VERTEX_SHADER:
        mRegistry.get().emplace<VertexShaderAsset>(mAssetHandle,
                                                   fileHandler->ReadFile(path));
        break;
      case ASSET_TYPE::TESC_SHADER:
        mRegistry.get().emplace<TessellationControlShaderAsset>(
          mAssetHandle, fileHandler->ReadFile(path));
        break;
      case ASSET_TYPE::TESE_SHADER:
        mRegistry.get().emplace<TessellationEvaluationShaderAsset>(
          mAssetHandle, fileHandler->ReadFile(path));
        break;
      case ASSET_TYPE::GEOMETRY_SHADER:
        mRegistry.get().emplace<GeometryShaderAsset>(
          mAssetHandle, fileHandler->ReadFile(path));
        break;
      case ASSET_TYPE::FRAGMENT_SHADER:
        mRegistry.get().emplace<FragmentShaderAsset>(
          mAssetHandle, fileHandler->ReadFile(path));
        break;
      case ASSET_TYPE::COMPUTE_SHADER:
        mRegistry.get().emplace<ComputeShaderAsset>(
          mAssetHandle, fileHandler->ReadFile(path));
        break;
      case ASSET_TYPE::HLSL_SHADER:
        mRegistry.get().emplace<HlslShaderAsset>(mAssetHandle,
                                                 fileHandler->ReadFile(path));
        break;
    }
  }

  /// @brief Retrieves the handle of the asset entity.
  /// @return The handle.
  auto
  AssetReference::GetHandle() const -> entt::entity
  {
    return mAssetHandle;
  }

  /// @brief Returns the UID of the asset.
  /// @return The UID.
  auto
  AssetReference::GetUID() const -> const UUID&
  {
    return mRegistry.get().get<IDComponent>(mAssetHandle).getId();
  }

  auto
  AssetReference::GetPath() const -> const std::filesystem::path&
  {
    return mRegistry.get().get<PathComponent>(mAssetHandle).getPath();
  }

  /// @brief Retrieves the asset component of the asset, containing the actual
  /// payload.
  /// @return The asset component.
  auto
  AssetReference::GetAsset() -> IAssetComponent&
  {
    switch (mType)
    {
      case ASSET_TYPE::TEXTURE:
        {
          TextureAsset& asset = mRegistry.get().get<TextureAsset>(mAssetHandle);
          std::filesystem::path path =
            mRegistry.get().get<PathComponent>(mAssetHandle).getPath();

          if (!asset.IsLoaded() && !mTextureLoadingWorker->IsRequested(path))
          {
            mTextureLoadingWorker->RequestTextureLoad({ &asset, path });
          }

          return asset;
        }
      case ASSET_TYPE::MODEL:
        return mRegistry.get().get<ModelAsset>(mAssetHandle);
      case ASSET_TYPE::MATERIAL:
        return mRegistry.get().get<MaterialAsset>(mAssetHandle);
      case ASSET_TYPE::UNKNOWN:
        return mRegistry.get().get<UnknownAsset>(mAssetHandle);
      case ASSET_TYPE::SCENE:
        return mRegistry.get().get<SceneAsset>(mAssetHandle);
      case ASSET_TYPE::VERTEX_SHADER:
        return mRegistry.get().get<VertexShaderAsset>(mAssetHandle);
      case ASSET_TYPE::TESC_SHADER:
        return mRegistry.get().get<TessellationControlShaderAsset>(
          mAssetHandle);
      case ASSET_TYPE::TESE_SHADER:
        return mRegistry.get().get<TessellationEvaluationShaderAsset>(
          mAssetHandle);
      case ASSET_TYPE::GEOMETRY_SHADER:
        return mRegistry.get().get<GeometryShaderAsset>(mAssetHandle);
      case ASSET_TYPE::FRAGMENT_SHADER:
        return mRegistry.get().get<FragmentShaderAsset>(mAssetHandle);
      case ASSET_TYPE::COMPUTE_SHADER:
        return mRegistry.get().get<ComputeShaderAsset>(mAssetHandle);
      case ASSET_TYPE::HLSL_SHADER:
        return mRegistry.get().get<HlslShaderAsset>(mAssetHandle);
      default: return mRegistry.get().get<UnknownAsset>(mAssetHandle);
    }
  }

  auto
  AssetReference::GetType() const -> ASSET_TYPE
  {
    return mType;
  }
}