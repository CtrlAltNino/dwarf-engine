#pragma once

#include "Core/Asset/AssetReference/IAssetReference.hpp"
#include "Core/Asset/AssetReference/IAssetReferenceFactory.hpp"
#include "Core/Asset/AssetTypes.hpp"
#include "Core/Asset/Model/IModelImporter.hpp"
#include "Core/Asset/Texture/TextureWorker/ITextureLoadingWorker.hpp"
#include "Core/Rendering/Material/IO/IMaterialIO.hpp"
#include "Core/Rendering/Texture/ITextureFactory.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "Utilities/FileHandler/IFileHandler.hpp"
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
    Create(entt::entity    assetHandle,
           entt::registry& registry,
           ASSET_TYPE      type) -> std::unique_ptr<IAssetReference> override;

    auto
    CreateNew(entt::entity          assetHandle,
              entt::registry&       registry,
              const UUID&           uid,
              std::filesystem::path path,
              std::string name) -> std::unique_ptr<IAssetReference> override;
  };
}