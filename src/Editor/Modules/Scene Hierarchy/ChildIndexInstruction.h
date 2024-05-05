#pragma once

#include "Editor/Modules/Scene Hierarchy/GraphInstruction.h"

namespace Dwarf
{

  /// @brief Buffered instruction for changing the child index of entities.
  class ChildIndexInstruction : public GraphInstruction
  {
  private:
    /// @brief Pointer to the currently opened scene.
    std::shared_ptr<Scene> m_Scene;

    /// @brief The entities to process.
    std::vector<Entity> m_SourceEntities;

    /// @brief Index to change the entities child index to.
    int m_Index;

  public:
    /// @brief Executes the instruction.
    void
    PerformInstruction() override;
    ChildIndexInstruction(std::shared_ptr<Scene>     scene,
                          std::vector<Entity> const& sourceEntities,
                          int                        index);
  };
}