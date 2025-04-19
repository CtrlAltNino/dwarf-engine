#pragma once

#include "Core/Scene/IScene.hpp"
#include "Editor/Modules/SceneHierarchy/GraphInstruction.hpp"
#include <entt/entt.hpp>
#include <functional>

namespace Dwarf
{

  /// @brief A buffered instruction to change the parent of entities.
  class NewParentInstruction : public GraphInstruction
  {
  private:
    /// @brief Pointer to the currently opened scene.
    std::reference_wrapper<IScene> mScene;

    /// @brief The entities to process the instruction on.
    std::vector<entt::entity> mSourceEntities;

    /// @brief The entity to be the new parent of the source entities.
    entt::entity mNewParent;

  public:
    NewParentInstruction(IScene&                   scene,
                         std::vector<entt::entity> sourceEntities,
                         entt::entity              newParent);

    /// @brief Executes the instruction.
    void
    PerformInstruction() override;
  };
}