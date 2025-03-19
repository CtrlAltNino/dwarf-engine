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

    virtual auto
    GetHandle() const -> entt::entity = 0;

    virtual auto
    GetUID() const -> const UUID& = 0;

    virtual auto
    GetPath() const -> const std::filesystem::path& = 0;

    virtual auto
    GetAsset() -> IAssetComponent& = 0;

    virtual auto
    GetType() const -> ASSET_TYPE = 0;
  };
}