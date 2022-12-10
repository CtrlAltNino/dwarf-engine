#pragma once

#include<entt/entt.hpp>

/// @brief Interface to only provide access to an ECS registry.
class EntityProvider {
	public:
        entt::registry m_Registry;
};