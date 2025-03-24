#include "pch.h"

#include "Editor/Modules/SceneHierarchy/DeleteEntityInstruction.h"

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
      Entity entity(ent, mScene.get().GetRegistry());
      mScene.get().DeleteEntity(entity);
    }
  }
}