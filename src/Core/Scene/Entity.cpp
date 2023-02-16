#include "Core/Scene/Entity.h"

namespace Dwarf {

    Entity::Entity(entt::entity handle, EntityProvider* entProvider)
        : entityHandle(handle),
            entProvider(entProvider){}
}