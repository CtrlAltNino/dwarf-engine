#include "pch.h"
#include "Editor/Modules/SceneHierarchy/DeleteEntityInstruction.h"

namespace Dwarf
{

  DeleteEntityInstruction::DeleteEntityInstruction(
    IScene&             scene,
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
      m_Scene.DeleteEntity(ent);
    }
  }
}