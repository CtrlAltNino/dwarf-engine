#pragma once

#include "Editor/Modules/Scene Hierarchy/GraphInstruction.h"
#include "Core/Scene/Scene.h"

namespace Dwarf
{

  /// @brief Buffered instruction for changing the child index of entities.
  class DeleteEntityInstruction : public GraphInstruction
  {
  private:
    /// @brief Pointer to the currently opened scene.
    std::shared_ptr<Scene> m_Scene;

    /// @brief The entities to delete.
    std::vector<Entity> m_SourceEntities;

  public:
    /// @brief Executes the instruction.
    void
    PerformInstruction() override;
    DeleteEntityInstruction(std::shared_ptr<Scene> scene,
                            std::vector<Entity>    sourceEntities);
  };
}