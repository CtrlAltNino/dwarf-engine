#include "pch.h"
#include "Editor/Modules/SceneHierarchy/NewParentInstruction.h"

namespace Dwarf
{

  NewParentInstruction::NewParentInstruction(
    IScene&                   scene,
    std::vector<entt::entity> sourceEntities,
    entt::entity              newParent)
    : m_Scene(scene)
    , m_SourceEntities(sourceEntities)
    , m_NewParent(newParent)
  {
  }

  void
  NewParentInstruction::PerformInstruction()
  {
    auto p = Entity(m_NewParent, m_Scene.GetRegistry());
    for (auto& ent : m_SourceEntities)
    {
      Entity entity(ent, m_Scene.GetRegistry());
      if ((p.GetComponent<TransformComponent>().GetParent() !=
           entity.GetHandle()) &&
          (entity.GetHandle() != m_NewParent))
      {
        entity.SetParent(m_NewParent);
      }
    }
  }
}