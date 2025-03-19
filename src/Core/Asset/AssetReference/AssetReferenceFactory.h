#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/AssetReference/IAssetReferenceFactory.h"
#include "Core/Asset/AssetTypes.h"
#include "Core/Asset/Model/IModelImporter.h"
#include "Core/Asset/Texture/TextureWorker/ITextureLoadingWorker.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include <memory>

namespace Dwarf
{
  class AssetReferenceFactory : public IAssetReferenceFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>          mLogger;
    std::shared_ptr<IModelImporter>        mModelImporter;
    std::shared_ptr<ITextureFactory>       mTextureFactory;
    std::shared_ptr<IMaterialIO>           mMaterialIo;
    std::shared_ptr<IFileHandler>          mFileHandler;
    std::shared_ptr<ITextureLoadingWorker> mTextureLoadingWorker;

  public:
    AssetReferenceFactory(
      std::shared_ptr<IDwarfLogger>          logger,
      std::shared_ptr<IModelImporter>        modelImporter,
      std::shared_ptr<ITextureFactory>       textureFactory,
      std::shared_ptr<IMaterialIO>           materialIO,
      std::shared_ptr<IFileHandler>          fileHandler,
      std::shared_ptr<ITextureLoadingWorker> textureLoadingWorker);
    ~AssetReferenceFactory() override;

    auto
    Create(entt::entity assetHandle, entt::registry& registry, ASSET_TYPE type)
      -> std::unique_ptr<IAssetReference> override;

    auto
    CreateNew(entt::entity          assetHandle,
              entt::registry&       registry,
              const UUID&           uid,
              std::filesystem::path path,
              std::string name) -> std::unique_ptr<IAssetReference> override;
  };
}