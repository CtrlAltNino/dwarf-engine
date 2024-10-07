#pragma once

#include "Editor/Modules/SceneHierarchy/GraphInstruction.h"
#include "Core/Scene/IScene.h"

namespace Dwarf
{

  /// @brief Buffered instruction for changing the child index of entities.
  class DeleteEntityInstruction : public GraphInstruction
  {
  private:
    /// @brief Pointer to the currently opened scene.
    IScene& m_Scene;

    /// @brief The entities to delete.
    std::vector<entt::entity> m_SourceEntities;

  public:
    /// @brief Executes the instruction.
    void
    PerformInstruction() override;
    DeleteEntityInstruction(IScene&                   scene,
                            std::vector<entt::entity> sourceEntities);
  };
}