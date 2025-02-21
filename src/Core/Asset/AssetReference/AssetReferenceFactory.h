#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/AssetReference/IAssetReferenceFactory.h"
#include "Core/Asset/AssetTypes.h"
#include "Core/Asset/Model/IModelImporter.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include "Core/Asset/Texture/TextureWorker/ITextureLoadingWorker.h"
#include <memory>

namespace Dwarf
{
  class AssetReferenceFactory : public IAssetReferenceFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>          m_Logger;
    std::shared_ptr<IModelImporter>        m_ModelImporter;
    std::shared_ptr<ITextureFactory>       m_TextureFactory;
    std::shared_ptr<IMaterialIO>           m_MaterialIO;
    std::shared_ptr<IFileHandler>          m_FileHandler;
    std::shared_ptr<ITextureLoadingWorker> m_TextureLoadingWorker;

  public:
    AssetReferenceFactory(
      std::shared_ptr<IDwarfLogger>          logger,
      std::shared_ptr<IModelImporter>        modelImporter,
      std::shared_ptr<ITextureFactory>       textureFactory,
      std::shared_ptr<IMaterialIO>           materialIO,
      std::shared_ptr<IFileHandler>          fileHandler,
      std::shared_ptr<ITextureLoadingWorker> textureLoadingWorker);
    ~AssetReferenceFactory() override = default;

    std::unique_ptr<IAssetReference>
    Create(entt::entity    assetHandle,
           entt::registry& registry,
           ASSET_TYPE      type) override;

    std::unique_ptr<IAssetReference>
    CreateNew(entt::entity          assetHandle,
              entt::registry&       registry,
              const UUID&           uid,
              std::filesystem::path path,
              std::string           name) override;
  };
}