#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/AssetTypes.h"
#include "Core/Asset/Model/IModelImporter.h"
#include "Core/Asset/Texture/TextureWorker/ITextureLoadingWorker.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"

namespace Dwarf
{
  /**
   * @brief Implementation of the class to interface with the asset database
   * entries
   *
   */
  class AssetReference : public IAssetReference
  {
  private:
    entt::entity                           mAssetHandle;
    std::reference_wrapper<entt::registry> mRegistry;
    ASSET_TYPE                             mType;
    std::shared_ptr<IDwarfLogger>          mLogger;
    std::shared_ptr<ITextureLoadingWorker> mTextureLoadingWorker;

  public:
    // Used for existing assets
    AssetReference(entt::entity                           assetHandle,
                   entt::registry&                        registry,
                   ASSET_TYPE                             type,
                   std::shared_ptr<IDwarfLogger>          logger,
                   std::shared_ptr<ITextureLoadingWorker> textureLoadingWorker);

    // Used for new assets
    AssetReference(entt::entity                            assetHandle,
                   entt::registry&                         registry,
                   UUID                                    uid,
                   std::filesystem::path                   path,
                   std::string                             name,
                   std::shared_ptr<IDwarfLogger>           logger,
                   std::shared_ptr<ITextureLoadingWorker>  textureLoadingWorker,
                   const std::shared_ptr<IModelImporter>&  modelImporter,
                   const std::shared_ptr<ITextureFactory>& textureFactory,
                   const std::shared_ptr<IMaterialIO>&     materialIO,
                   const std::shared_ptr<IFileHandler>&    fileHandler);

    ~AssetReference() override = default;

    operator bool() const { return (std::uint32_t)mAssetHandle != 0; }

    auto
    operator==(const AssetReference& other) -> bool
    {
      return mAssetHandle == other.mAssetHandle;
    }

    [[nodiscard]] auto
    GetHandle() const -> entt::entity override;

    [[nodiscard]] auto
    GetUID() const -> const UUID& override;

    [[nodiscard]] auto
    GetPath() const -> const std::filesystem::path& override;

    auto
    GetAsset() -> IAssetComponent& override;

    [[nodiscard]] auto
    GetType() const -> ASSET_TYPE override;

    /**
     * @brief Checks if this AssetReference is still valid
     *
     * @return true If it's valid
     * @return false If it's not present in the registry
     */
    [[nodiscard]] auto
    IsValid() const -> bool override;
  };
}