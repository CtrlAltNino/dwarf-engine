#pragma once

#include "Core/UUID.h"
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>

namespace Dwarf
{
  template<typename T>
  class IAssetReference
  {
  public:
    virtual entt::entity
    GetHandle() const;

    template<typename U = T, typename... Args>
    U&
    AddAssetComponent(Args&&... args);

    virtual const UUID&
    GetUID() const;

    virtual const std::filesystem::path&
    GetPath() const;

    virtual T&
    GetAsset();
  };
}