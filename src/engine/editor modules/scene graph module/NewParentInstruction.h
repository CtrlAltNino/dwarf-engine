#pragma once

#include "GraphInstruction.h"
#include "../../data structures/Entity.h"

/// @brief A buffered instruction to change the parent of entities.
class NewParentInstruction : public GraphInstruction {
    private:
        /// @brief The entities to process the instruction on.
        std::vector<Entity> sourceEntities;

        /// @brief The entity to be the new parent of the source entities.
        entt::entity newParent;
    public:
        /// @brief Executes the instruction.
        virtual void PerformInstruction();
        NewParentInstruction(Scene* scene, std::vector<Entity> sourceEntities, entt::entity newParent);
};