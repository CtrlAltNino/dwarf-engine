#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/AssetTypes.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Model/IModelImporter.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Rendering/Texture/ITextureFactory.h"

namespace Dwarf
{
  class AssetReference : public IAssetReference
  {
  private:
    entt::entity    m_AssetHandle;
    entt::registry& m_Registry;
    ASSET_TYPE      m_Type;

    std::shared_ptr<IModelImporter>  m_ModelImporter;
    std::shared_ptr<ITextureFactory> m_TextureFactory;
    std::shared_ptr<IMaterialIO>     m_MaterialIO;

  public:
    AssetReference(entt::entity                     assetHandle,
                   entt::registry&                  registry,
                   ASSET_TYPE                       type,
                   std::shared_ptr<IModelImporter>  modelImporter,
                   std::shared_ptr<ITextureFactory> textureFactory,
                   std::shared_ptr<IMaterialIO>     materialIO);

    AssetReference(entt::entity                     assetHandle,
                   entt::registry&                  registry,
                   UUID                             uid,
                   std::filesystem::path            path,
                   std::string                      name,
                   std::shared_ptr<IModelImporter>  modelImporter,
                   std::shared_ptr<ITextureFactory> textureFactory,
                   std::shared_ptr<IMaterialIO>     materialIO);

    ~AssetReference() override = default;

    operator bool() const { return (std::uint32_t)m_AssetHandle != 0; }

    bool
    operator==(const AssetReference& b)
    {
      return m_AssetHandle == b.m_AssetHandle;
    }

    entt::entity
    GetHandle() const override;

    // template<typename T, typename... Args>
    // T&
    // AddAssetComponent(Args&&... args);

    const UUID&
    GetUID() const override;

    const std::filesystem::path&
    GetPath() const override;

    IAssetComponent&
    GetAsset() override;

    ASSET_TYPE
    GetType() const override;
  };
}