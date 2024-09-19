#include "AssetReferenceFactory.h"
#include "AssetReference.h"
#include "Core/Asset/AssetReference/IAssetReference.h"

namespace Dwarf
{
  AssetReferenceFactory::AssetReferenceFactory(
    std::shared_ptr<IDwarfLogger>    logger,
    std::shared_ptr<IModelImporter>  modelImporter,
    std::shared_ptr<ITextureFactory> textureFactory,
    std::shared_ptr<IMaterialIO>     materialIO)
    : m_Logger(logger)
    , m_ModelImporter(modelImporter)
    , m_TextureFactory(textureFactory)
    , m_MaterialIO(materialIO)
  {
    m_Logger->LogInfo(
      Log("AssetReferenceFactory created", "AssetReferenceFactory"));
  }

  std::unique_ptr<IAssetReference>
  AssetReferenceFactory::Create(entt::entity    assetHandle,
                                entt::registry& registry,
                                ASSET_TYPE      type)
  {
    m_Logger->LogInfo(Log("Creating asset reference for existing asset",
                          "AssetReferenceFactory"));
    return std::make_unique<AssetReference>(assetHandle,
                                            registry,
                                            type,
                                            m_ModelImporter,
                                            m_TextureFactory,
                                            m_MaterialIO);
  }

  std::unique_ptr<IAssetReference>
  AssetReferenceFactory::CreateNew(entt::entity          assetHandle,
                                   entt::registry&       registry,
                                   const UUID&           uid,
                                   std::filesystem::path path,
                                   std::string           name)
  {
    m_Logger->LogInfo(
      Log(fmt::format("Creating asset reference for new asset at path: {}",
                      path.string()),
          "AssetReferenceFactory"));
    return std::make_unique<AssetReference>(assetHandle,
                                            registry,
                                            uid,
                                            path,
                                            name,
                                            m_ModelImporter,
                                            m_TextureFactory,
                                            m_MaterialIO);
  }
}