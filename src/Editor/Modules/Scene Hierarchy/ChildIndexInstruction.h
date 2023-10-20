#pragma once

#include "Editor/Modules/Scene Hierarchy/GraphInstruction.h"

namespace Dwarf
{

    /// @brief Buffered instruction for changing the child index of entities.
    class ChildIndexInstruction : public GraphInstruction
    {
    private:
        /// @brief The entities to process.
        std::vector<Entity> m_SourceEntities;

        /// @brief Index to change the entities child index to.
        int m_Index;

    public:
        /// @brief Executes the instruction.
        virtual void PerformInstruction();
        ChildIndexInstruction(Ref<Scene> scene, std::vector<Entity> sourceEntities, int index);
    };
}