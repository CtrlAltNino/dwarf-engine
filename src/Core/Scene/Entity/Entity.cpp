#include "Entity.h"
#include "pch.h"

namespace Dwarf
{

  Entity::Entity(const entt::entity handle, entt::registry& registry)
    : mEntityHandle(handle)
    , mRegistry(registry)
  {
  }
}