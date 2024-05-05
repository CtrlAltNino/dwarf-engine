#include "dpch.h"
#include "Core/Scene/Entity.h"

namespace Dwarf
{

  Entity::Entity(entt::entity handle, std::shared_ptr<entt::registry> registry)
    : m_EntityHandle(handle)
    , m_Registry(registry)
  {
  }
}