#include "pch.hpp"

#include "Entity.hpp"

namespace Dwarf
{

  Entity::Entity(const entt::entity handle, entt::registry& registry)
    : mEntityHandle(handle)
    , mRegistry(registry)
  {
  }
}