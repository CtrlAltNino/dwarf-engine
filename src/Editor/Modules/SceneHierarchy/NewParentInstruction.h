#pragma once

#include "Core/Scene/Entity/Entity.h"
#include "Core/Scene/Scene.h"
#include "Editor/Modules/SceneHierarchy/GraphInstruction.h"

namespace Dwarf
{

  /// @brief A buffered instruction to change the parent of entities.
  class NewParentInstruction : public GraphInstruction
  {
  private:
    /// @brief Pointer to the currently opened scene.
    IScene& mScene;

    /// @brief The entities to process the instruction on.
    std::vector<entt::entity> mSourceEntities;

    /// @brief The entity to be the new parent of the source entities.
    entt::entity mNewParent;

  public:
    /// @brief Executes the instruction.
    void
    PerformInstruction() override;
    NewParentInstruction(IScene&                   scene,
                         std::vector<entt::entity> sourceEntities,
                         entt::entity              newParent);
  };
}