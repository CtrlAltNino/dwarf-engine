#pragma once

#include "Core/Scene/IScene.hpp"
#include "Editor/Modules/SceneHierarchy/GraphInstruction.hpp"
#include <functional>

namespace Dwarf
{

  /// @brief Buffered instruction for changing the child index of entities.
  class ChildIndexInstruction : public GraphInstruction
  {
  private:
    /// @brief Pointer to the currently opened scene.
    std::reference_wrapper<IScene> mScene;

    /// @brief The entities to process.
    std::vector<entt::entity> mSourceEntities;

    /// @brief Index to change the entities child index to.
    int mIndex;

  public:
    ChildIndexInstruction(IScene&                   scene,
                          std::vector<entt::entity> sourceEntities,
                          int                       index);

    /// @brief Executes the instruction.
    void
    PerformInstruction() override;
  };
}