#include "pch.h"
#include "Editor/Modules/SceneHierarchy/ChildIndexInstruction.h"

namespace Dwarf
{

  ChildIndexInstruction::ChildIndexInstruction(
    IScene&                          scene,
    std::vector<entt::entity> const& sourceEntities,
    int                              index)
    : m_Scene(scene)
    , m_SourceEntities(sourceEntities)
    , m_Index(index)
  {
  }

  void
  ChildIndexInstruction::PerformInstruction()
  {
    int startIndex = m_Index;
    for (auto& ent : m_SourceEntities)
    {
      Entity entity(ent, m_Scene.GetRegistry());
      entity.SetChildIndex(startIndex);
      startIndex++;
    }
  }
}