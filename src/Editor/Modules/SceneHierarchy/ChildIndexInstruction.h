#pragma once

#include "Core/Scene/Scene.h"
#include "Editor/Modules/SceneHierarchy/GraphInstruction.h"

namespace Dwarf
{

  /// @brief Buffered instruction for changing the child index of entities.
  class ChildIndexInstruction : public GraphInstruction
  {
  private:
    /// @brief Pointer to the currently opened scene.
    IScene& mScene;

    /// @brief The entities to process.
    std::vector<entt::entity> mSourceEntities;

    /// @brief Index to change the entities child index to.
    int mIndex;

  public:
    /// @brief Executes the instruction.
    void
    PerformInstruction() override;
    ChildIndexInstruction(IScene&                          scene,
                          std::vector<entt::entity> const& sourceEntities,
                          int                              index);
  };
}