#include "Editor/Modules/SceneHierarchy/ChildIndexInstruction.h"
#include "pch.h"

namespace Dwarf
{

  ChildIndexInstruction::ChildIndexInstruction(
    IScene&                          scene,
    std::vector<entt::entity> const& sourceEntities,
    int                              index)
    : mScene(scene)
    , mSourceEntities(sourceEntities)
    , mIndex(index)
  {
  }

  void
  ChildIndexInstruction::PerformInstruction()
  {
    int startIndex = mIndex;
    for (auto& ent : mSourceEntities)
    {
      Entity entity(ent, mScene.GetRegistry());
      entity.SetChildIndex(startIndex);
      startIndex++;
    }
  }
}