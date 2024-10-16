#include "Editor/Modules/Scene Hierarchy/NewParentInstruction.h"

namespace Dwarf
{

  NewParentInstruction::NewParentInstruction(
    std::shared_ptr<Scene>     scene,
    std::vector<Entity> const& sourceEntities,
    entt::entity               newParent)
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
      if ((p.GetComponent<TransformComponent>().parent != ent.GetHandle()) &&
          (ent.GetHandle() != m_NewParent))
      {
        ent.SetParent(m_NewParent);
      }
    }
  }
}