#include "pch.h"
#include "Editor/Modules/SceneHierarchy/DeleteEntityInstruction.h"

namespace Dwarf
{

  DeleteEntityInstruction::DeleteEntityInstruction(
    IScene&                   scene,
    std::vector<entt::entity> sourceEntities)
    : m_Scene(scene)
    , m_SourceEntities(sourceEntities)
  {
  }

  void
  DeleteEntityInstruction::PerformInstruction()
  {
    for (auto& ent : m_SourceEntities)
    {
      Entity entity(ent, m_Scene.GetRegistry());
      m_Scene.DeleteEntity(entity);
    }
  }
}