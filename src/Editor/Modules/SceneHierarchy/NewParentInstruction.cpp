#include "Editor/Modules/SceneHierarchy/NewParentInstruction.h"
#include "pch.h"

namespace Dwarf
{

  NewParentInstruction::NewParentInstruction(
    IScene&                   scene,
    std::vector<entt::entity> sourceEntities,
    entt::entity              newParent)
    : mScene(scene)
    , mSourceEntities(sourceEntities)
    , mNewParent(newParent)
  {
  }

  void
  NewParentInstruction::PerformInstruction()
  {
    auto p = Entity(mNewParent, mScene.GetRegistry());
    for (auto& ent : mSourceEntities)
    {
      Entity entity(ent, mScene.GetRegistry());
      if ((p.GetComponent<TransformComponent>().GetParent() !=
           entity.GetHandle()) &&
          (entity.GetHandle() != mNewParent))
      {
        entity.SetParent(mNewParent);
      }
    }
  }
}