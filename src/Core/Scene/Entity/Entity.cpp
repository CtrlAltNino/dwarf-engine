#include "pch.h"
#include "Entity.h"

namespace Dwarf
{

  Entity::Entity(const entt::entity handle, entt::registry& registry)
    : m_EntityHandle(handle)
    , m_Registry(registry)
  {
  }
}