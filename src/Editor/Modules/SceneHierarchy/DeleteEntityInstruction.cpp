#include "Editor/Modules/SceneHierarchy/DeleteEntityInstruction.h"
#include "pch.h"

namespace Dwarf
{

  DeleteEntityInstruction::DeleteEntityInstruction(
    IScene&                   scene,
    std::vector<entt::entity> sourceEntities)
    : mScene(scene)
    , mSourceEntities(sourceEntities)
  {
  }

  void
  DeleteEntityInstruction::PerformInstruction()
  {
    for (auto& ent : mSourceEntities)
    {
      Entity entity(ent, mScene.GetRegistry());
      mScene.DeleteEntity(entity);
    }
  }
}