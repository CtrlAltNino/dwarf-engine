#pragma once

#include "Core/GenericComponents.hpp"
#include "Core/Scene/Components/SceneComponents.hpp"

namespace Dwarf
{
  class NameComponentHandle
  {
  private:
    entt::registry& mRegistry;
    entt::entity    mEntity;

  public:
    NameComponentHandle(entt::registry& reg, entt::entity ent)
      : mRegistry(reg)
      , mEntity(ent)
    {
    }

    [[nodiscard]] auto
    GetName() const -> std::string
    {
      return mRegistry.get<NameComponent>(mEntity).Name;
    }

    void
    SetName(std::string name)
    {
      mRegistry.patch<NameComponent>(mEntity,
                                     [name](NameComponent& component) mutable
                                     { component.Name = name; });
    }
  };
}