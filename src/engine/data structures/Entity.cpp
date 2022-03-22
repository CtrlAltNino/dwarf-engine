#include"Entity.h"

Entity::Entity(entt::entity handle, EntityProvider* entProvider)
    : entityHandle(handle),
        entProvider(entProvider){}