#include "AssetReferenceFactory.h"
#include "AssetReference.h"
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
    : m_Logger(logger)
    , m_ModelImporter(modelImporter)
    , m_TextureFactory(textureFactory)
    , m_MaterialIO(materialIO)
    , m_FileHandler(fileHandler)
    , m_TextureLoadingWorker(textureLoadingWorker)
  {
    m_Logger->LogDebug(
      Log("AssetReferenceFactory created", "AssetReferenceFactory"));
  }

  AssetReferenceFactory::~AssetReferenceFactory()
  {
    m_Logger->LogDebug(
      Log("AssetReferenceFactory destroyed", "AssetReferenceFactory"));
  }

  std::unique_ptr<IAssetReference>
  AssetReferenceFactory::Create(entt::entity    assetHandle,
                                entt::registry& registry,
                                ASSET_TYPE      type)
  {
    return std::make_unique<AssetReference>(
      assetHandle, registry, type, m_TextureLoadingWorker, m_Logger);
  }

  std::unique_ptr<IAssetReference>
  AssetReferenceFactory::CreateNew(entt::entity          assetHandle,
                                   entt::registry&       registry,
                                   const UUID&           uid,
                                   std::filesystem::path path,
                                   std::string           name)
  {
    return std::make_unique<AssetReference>(assetHandle,
                                            registry,
                                            uid,
                                            path,
                                            name,
                                            m_Logger,
                                            m_ModelImporter,
                                            m_TextureFactory,
                                            m_MaterialIO,
                                            m_FileHandler,
                                            m_TextureLoadingWorker);
  }
}