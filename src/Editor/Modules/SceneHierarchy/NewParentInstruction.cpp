#include "pch.h"

#include "Editor/Modules/SceneHierarchy/NewParentInstruction.h"

namespace Dwarf
{

  NewParentInstruction::NewParentInstruction(
    IScene&                   scene,
    std::vector<entt::entity> sourceEntities,
    entt::entity              newParent)
    : mScene(scene)
    , mSourceEntities(std::move(sourceEntities))
    , mNewParent(newParent)
  {
  }

  void
  NewParentInstruction::PerformInstruction()
  {
    auto newParentEntity = Entity(mNewParent, mScene.get().GetRegistry());
    for (auto& ent : mSourceEntities)
    {
      Entity entity(ent, mScene.get().GetRegistry());
      if ((newParentEntity.GetComponent<TransformComponent>().GetParent() !=
           entity.GetHandle()) &&
          (entity.GetHandle() != mNewParent))
      {
        entity.SetParent(mNewParent);
      }
    }
  }
}