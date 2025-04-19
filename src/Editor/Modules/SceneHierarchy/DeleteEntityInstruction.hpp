#pragma once

#include "Core/Scene/IScene.hpp"
#include "Editor/Modules/SceneHierarchy/GraphInstruction.hpp"

namespace Dwarf
{

  /// @brief Buffered instruction for changing the child index of entities.
  class DeleteEntityInstruction : public GraphInstruction
  {
  private:
    /// @brief Pointer to the currently opened scene.
    std::reference_wrapper<IScene> mScene;

    /// @brief The entities to delete.
    std::vector<entt::entity> mSourceEntities;

  public:
    DeleteEntityInstruction(IScene&                   scene,
                            std::vector<entt::entity> sourceEntities);

    /// @brief Executes the instruction.
    void
    PerformInstruction() override;
  };
}