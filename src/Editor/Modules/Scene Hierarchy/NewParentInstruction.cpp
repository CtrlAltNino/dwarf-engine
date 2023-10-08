#include "Editor/Modules/Scene Hierarchy/NewParentInstruction.h"

namespace Dwarf
{

    NewParentInstruction::NewParentInstruction(Ref<Scene> scene, std::vector<Entity> sourceEntities, entt::entity newParent)
        : GraphInstruction(scene), m_SourceEntities(sourceEntities), m_NewParent(newParent) {}

    void NewParentInstruction::PerformInstruction()
    {
        Entity p = Entity(m_NewParent, m_Scene->GetRegistry());
        for (Entity ent : m_SourceEntities)
        {
            if ((p.GetComponent<TransformComponent>().parent != ent.GetHandle()) && (ent.GetHandle() != m_NewParent))
            {
                ent.SetParent(m_NewParent);
            }
        }
    }
}