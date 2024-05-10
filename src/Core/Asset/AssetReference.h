#pragma once

#include <entt/entt.hpp>

#include "Core/Base.h"
#include "Core/UID.h"
#include "Core/GenericComponents.h"
#include <filesystem>
#include <string>

namespace Dwarf
{
  /// @brief Wrapper of an asset reference. It handles the access of the
  /// components.
  template<typename T>
  class AssetReference
  {
  private:
    /// @brief Registry handle of the corresponding entity.
    entt::entity m_AssetHandle;

    /// @brief Pointer to the holder of the ECS registry.
    std::shared_ptr<entt::registry> m_Registry;

  public:
    AssetReference(entt::entity                    assetHandle,
                   std::shared_ptr<entt::registry> registry)
      : m_AssetHandle(assetHandle)
      , m_Registry(registry)
    {
    }

    AssetReference(entt::entity                    assetHandle,
                   std::string                     assetName,
                   std::shared_ptr<entt::registry> registry,
                   UID                             uid,
                   std::filesystem::path           assetPath)
      : m_AssetHandle(assetHandle)
      , m_Registry(registry)
    {
      m_Registry->emplace<T>(assetHandle, assetPath);
      m_Registry->emplace<IDComponent>(assetHandle, uid);
      m_Registry->emplace<NameComponent>(assetHandle, assetName);
      m_Registry->emplace<PathComponent>(assetHandle, assetPath);
    }

    operator bool() const { return (std::uint32_t)m_AssetHandle != 0; }
    bool
    operator==(const AssetReference& b)
    {
      return m_AssetHandle == b.m_AssetHandle;
    }

    /// @brief Retrieves the handle of the asset entity.
    /// @return The handle.
    entt::entity
    GetHandle() const
    {
      return m_AssetHandle;
    }

    /// @brief Returns the UID of the asset.
    /// @return The UID.
    std::shared_ptr<UID>
    GetUID() const
    {
      return m_Registry->get<IDComponent>(m_AssetHandle).ID;
    }

    std::filesystem::path
    GetPath() const
    {
      return m_Registry->get<PathComponent>(m_AssetHandle).Path;
    }

    /// @brief Retrieves the asset component of the asset, containing the actual
    /// payload.
    /// @return The asset component.
    std::shared_ptr<T>
    GetAsset()
    {
      return std::make_shared<T>(m_Registry->get<T>(m_AssetHandle));
    }
  };
}