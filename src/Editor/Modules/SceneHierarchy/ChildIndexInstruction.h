#pragma once

#include "Editor/Modules/SceneHierarchy/GraphInstruction.h"
#include "Core/Scene/Scene.h"

namespace Dwarf
{

  /// @brief Buffered instruction for changing the child index of entities.
  class ChildIndexInstruction : public GraphInstruction
  {
  private:
    /// @brief Pointer to the currently opened scene.
    IScene& m_Scene;

    /// @brief The entities to process.
    std::vector<entt::entity> m_SourceEntities;

    /// @brief Index to change the entities child index to.
    int m_Index;

  public:
    /// @brief Executes the instruction.
    void
    PerformInstruction() override;
    ChildIndexInstruction(IScene&                          scene,
                          std::vector<entt::entity> const& sourceEntities,
                          int                              index);
  };
}