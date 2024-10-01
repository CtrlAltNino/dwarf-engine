#pragma once

#include "Core/Asset/AssetTypes.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/UUID.h"
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>

namespace Dwarf
{
  class IAssetReference
  {
  public:
    // IAssetReference() = delete;
    virtual ~IAssetReference() = default;

    virtual entt::entity
    GetHandle() const = 0;

    virtual const UUID&
    GetUID() const = 0;

    virtual const std::filesystem::path&
    GetPath() const = 0;

    virtual IAssetComponent&
    GetAsset() = 0;

    virtual ASSET_TYPE
    GetType() const = 0;

    virtual std::unique_ptr<IAssetReference>
    Clone() const = 0;
  };
}