#include "pch.h"
#include "Editor/Modules/SceneHierarchy/ChildIndexInstruction.h"

namespace Dwarf
{

  ChildIndexInstruction::ChildIndexInstruction(
    IScene&                    scene,
    std::vector<Entity> const& sourceEntities,
    int                        index)
    : m_Scene(scene)
    , m_SourceEntities(sourceEntities)
    , m_Index(index)
  {
  }

  void
  ChildIndexInstruction::PerformInstruction()
  {
    int startIndex = m_Index;
    for (Entity ent : m_SourceEntities)
    {
      ent.SetChildIndex(startIndex);
      startIndex++;
    }
  }
}