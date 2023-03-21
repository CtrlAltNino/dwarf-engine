#include "Core/Scene/Entity.h"

namespace Dwarf {

    Entity::Entity(entt::entity handle, Ref<entt::registry> registry)
        : m_EntityHandle(handle),
            m_Registry(registry){}
}