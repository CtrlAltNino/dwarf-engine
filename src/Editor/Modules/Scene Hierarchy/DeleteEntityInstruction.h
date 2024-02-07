#pragma once

#include "Editor/Modules/Scene Hierarchy/GraphInstruction.h"

namespace Dwarf
{

    /// @brief Buffered instruction for changing the child index of entities.
    class DeleteEntityInstruction : public GraphInstruction
    {
    private:
        /// @brief The entities to delete.
        std::vector<Entity> m_SourceEntities;

    public:
        /// @brief Executes the instruction.
        virtual void PerformInstruction();
        DeleteEntityInstruction(Ref<Scene> scene, std::vector<Entity> sourceEntities);
    };
}