#include "pch.h"
#include "Editor/Modules/SceneHierarchy/NewParentInstruction.h"

namespace Dwarf
{

  NewParentInstruction::NewParentInstruction(std::shared_ptr<IScene> scene,
                                             std::vector<Entity> sourceEntities,
                                             entt::entity        newParent)
    : m_Scene(scene)
    , m_SourceEntities(sourceEntities)
    , m_NewParent(newParent)
  {
  }

  void
  NewParentInstruction::PerformInstruction()
  {
    auto p = Entity(m_NewParent, m_Scene->GetRegistry());
    for (Entity ent : m_SourceEntities)
    {
      if ((p.GetComponent<TransformComponent>().GetParent() !=
           ent.GetHandle()) &&
          (ent.GetHandle() != m_NewParent))
      {
        ent.SetParent(m_NewParent);
      }
    }
  }
}