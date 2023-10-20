#include "Editor/Modules/Scene Hierarchy/ChildIndexInstruction.h"

namespace Dwarf
{

    ChildIndexInstruction::ChildIndexInstruction(Ref<Scene> scene, std::vector<Entity> sourceEntities, int index)
        : GraphInstruction(scene), m_SourceEntities(sourceEntities), m_Index(index) {}

    void ChildIndexInstruction::PerformInstruction()
    {
        int startIndex = m_Index;
        for (Entity ent : m_SourceEntities)
        {
            ent.SetChildIndex(startIndex++);
        }
    }
}