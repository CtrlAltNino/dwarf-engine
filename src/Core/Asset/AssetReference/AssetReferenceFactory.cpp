#include "pch.h"

#include "AssetReference.h"
#include "AssetReferenceFactory.h"
#include "Core/Asset/AssetReference/IAssetReference.h"

namespace Dwarf
{
  AssetReferenceFactory::AssetReferenceFactory(
    std::shared_ptr<IDwarfLogger>          logger,
    std::shared_ptr<IModelImporter>        modelImporter,
    std::shared_ptr<ITextureFactory>       textureFactory,
    std::shared_ptr<IMaterialIO>           materialIO,
    std::shared_ptr<IFileHandler>          fileHandler,
    std::shared_ptr<ITextureLoadingWorker> textureLoadingWorker)
    : mLogger(std::move(logger))
    , mModelImporter(std::move(modelImporter))
    , mTextureFactory(std::move(textureFactory))
    , mMaterialIo(std::move(materialIO))
    , mFileHandler(std::move(fileHandler))
    , mTextureLoadingWorker(std::move(textureLoadingWorker))
  {
    mLogger->LogDebug(
      Log("AssetReferenceFactory created", "AssetReferenceFactory"));
  }

  AssetReferenceFactory::~AssetReferenceFactory()
  {
    mLogger->LogDebug(
      Log("AssetReferenceFactory destroyed", "AssetReferenceFactory"));
  }

  auto
  AssetReferenceFactory::Create(entt::entity    assetHandle,
                                entt::registry& registry,
                                ASSET_TYPE      type)
    -> std::unique_ptr<IAssetReference>
  {
    return std::make_unique<AssetReference>(
      assetHandle, registry, type, mLogger, mTextureLoadingWorker);
  }

  auto
  AssetReferenceFactory::CreateNew(entt::entity          assetHandle,
                                   entt::registry&       registry,
                                   const UUID&           uid,
                                   std::filesystem::path path,
                                   std::string           name)
    -> std::unique_ptr<IAssetReference>
  {
    return std::make_unique<AssetReference>(assetHandle,
                                            registry,
                                            uid,
                                            path,
                                            name,
                                            mLogger,
                                            mTextureLoadingWorker,
                                            mModelImporter,
                                            mTextureFactory,
                                            mMaterialIo,
                                            mFileHandler);
  }
}