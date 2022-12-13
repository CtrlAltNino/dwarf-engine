#pragma once

#include "GraphInstruction.h"

/// @brief Buffered instruction for changing the child index of entities.
class ChildIndexInstruction : public GraphInstruction {
    private:
        /// @brief The entities to process.
        std::vector<Entity> sourceEntities;

        /// @brief Index to change the entities child index to.
        int index;
    public:
        /// @brief Executes the instruction.
        virtual void PerformInstruction();
        ChildIndexInstruction(Scene* scene, std::vector<Entity> sourceEntities, int index);
};