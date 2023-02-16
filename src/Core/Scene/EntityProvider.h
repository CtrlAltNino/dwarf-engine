#pragma once

#include <entt/entt.hpp>

namespace Dwarf {

    /// @brief Interface to only provide access to an ECS registry.
    class EntityProvider {
        public:
        entt::registry m_Registry;
    };
}