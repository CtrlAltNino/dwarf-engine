#pragma once

#include "Core/Asset/Database/AssetReference.h"
#include <typeindex>

namespace Dwarf
{
  class IAssetInspector
  {
  public:
    // template<typename T>
    // void
    // Render(std::shared_ptr<AssetReference<T>> asset)
    // {
    //   return std::static_pointer_cast<AssetReference<T>>(
    //     RenderImpl(typeid(T), asset));
    // }

    virtual void
    Render(const std::filesystem::path& assetPath) = 0;

  private:
    // NVI (Non-Virtual Interface) Implementations
    // virtual void
    // RenderImpl(std::type_index type, std::shared_ptr<UUID> uid) const = 0;
  };
}