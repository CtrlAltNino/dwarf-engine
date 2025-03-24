#include "pch.h"

#include "Editor/Modules/SceneHierarchy/ChildIndexInstruction.h"

namespace Dwarf
{

  ChildIndexInstruction::ChildIndexInstruction(
    IScene&                   scene,
    std::vector<entt::entity> sourceEntities,
    int                       index)
    : mScene(scene)
    , mSourceEntities(std::move(sourceEntities))
    , mIndex(index)
  {
  }

  void
  ChildIndexInstruction::PerformInstruction()
  {
    int startIndex = mIndex;
    for (auto& ent : mSourceEntities)
    {
      Entity entity(ent, mScene.get().GetRegistry());
      entity.SetChildIndex(startIndex);
      startIndex++;
    }
  }
}