#include "Editor/Modules/Scene Hierarchy/DeleteEntityInstruction.h"

namespace Dwarf
{

  DeleteEntityInstruction::DeleteEntityInstruction(
    Ref<Scene>          scene,
    std::vector<Entity> sourceEntities)
    : m_Scene(scene)
    , m_SourceEntities(sourceEntities)
  {
  }

  void
  DeleteEntityInstruction::PerformInstruction()
  {
    for (Entity ent : m_SourceEntities)
    {
      m_Scene->DeleteEntity(ent);
    }
  }
}