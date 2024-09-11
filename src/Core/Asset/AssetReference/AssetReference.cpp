#include "AssetReference.h"
#include "Core/GenericComponents.h"

namespace Dwarf
{
  template<typename T>
  AssetReference<T>::AssetReference(entt::entity                    assetHandle,
                                    std::shared_ptr<entt::registry> registry)
    : m_AssetHandle(assetHandle)
    , m_Registry(registry)
  {
  }

  /// @brief Retrieves the handle of the asset entity.
  /// @return The handle.
  template<typename T>
  entt::entity
  AssetReference<T>::GetHandle() const
  {
    return m_AssetHandle;
  }
  template<typename T>
  template<typename U, typename... Args>
  U&
  AssetReference<T>::AddAssetComponent(Args&&... args)
  {
    // TODO: Check component requirements
    return m_Registry->emplace<U>(m_AssetHandle, std::forward<Args>(args)...);
  }

  /// @brief Returns the UID of the asset.
  /// @return The UID.
  template<typename T>
  const UUID&
  AssetReference<T>::GetUID() const
  {
    return m_Registry->get<IDComponent>(m_AssetHandle).GetID();
  }

  template<typename T>
  const std::filesystem::path&
  AssetReference<T>::GetPath() const
  {
    return m_Registry->get<PathComponent>(m_AssetHandle).GetPath();
  }

  /// @brief Retrieves the asset component of the asset, containing the actual
  /// payload.
  /// @return The asset component.
  template<typename T>
  T&
  AssetReference<T>::GetAsset()
  {
    return m_Registry->get<T>(m_AssetHandle);
  }

}