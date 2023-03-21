#pragma once

#include "Core/Base.h"
#include "Editor/Modules/Scene Hierarchy/GraphInstruction.h"
#include "Core/Scene/Entity.h"

namespace Dwarf {

    /// @brief A buffered instruction to change the parent of entities.
    class NewParentInstruction : public GraphInstruction {
        private:
            /// @brief The entities to process the instruction on.
            std::vector<Entity> m_SourceEntities;

            /// @brief The entity to be the new parent of the source entities.
            entt::entity m_NewParent;
        public:
            /// @brief Executes the instruction.
            virtual void PerformInstruction();
            NewParentInstruction(Ref<Scene> scene, std::vector<Entity> sourceEntities, entt::entity newParent);
    };
}