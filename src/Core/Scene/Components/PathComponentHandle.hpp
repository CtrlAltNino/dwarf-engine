#pragma once

#include "Core/GenericComponents.hpp"
#include "Core/Scene/Components/SceneComponents.hpp"

namespace Dwarf
{
  class PathComponentHandle
  {
  private:
    entt::registry& mRegistry;
    entt::entity    mEntity;

  public:
  PathComponentHandle(entt::registry& reg, entt::entity ent)
      : mRegistry(reg)
      , mEntity(ent)
    {
    }

    [[nodiscard]] auto
    GetPath() const -> std::filesystem::path
    {
      return mRegistry.get<PathComponent>(mEntity).Path;
    }

    void
    SetPath(std::filesystem::path path)
    {
      mRegistry.patch<PathComponent>(mEntity,
                                     [path](PathComponent& component) mutable
                                     { component.Path = path; });
    }
  };
}