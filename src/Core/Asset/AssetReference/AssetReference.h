#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"

namespace Dwarf
{
  template<typename T>
  class AssetReference : public IAssetReference<T>
  {
  private:
    entt::entity                    m_AssetHandle;
    std::shared_ptr<entt::registry> m_Registry;

  public:
    AssetReference(entt::entity                    assetHandle,
                   std::shared_ptr<entt::registry> registry);

    operator bool() const { return (std::uint32_t)m_AssetHandle != 0; }
    bool
    operator==(const AssetReference& b)
    {
      return m_AssetHandle == b.m_AssetHandle;
    }

    entt::entity
    GetHandle() const override;

    template<typename U = T, typename... Args>
    U&
    AddAssetComponent(Args&&... args);

    const UUID&
    GetUID() const override;

    const std::filesystem::path&
    GetPath() const override;

    T&
    GetAsset() override;
  };
}