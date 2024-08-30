#pragma once

#include "Editor/Modules/SceneHierarchy/GraphInstruction.h"
#include "Core/Scene/Entity/Entity.h"
#include "Core/Scene/Scene.h"

namespace Dwarf
{

  /// @brief A buffered instruction to change the parent of entities.
  class NewParentInstruction : public GraphInstruction
  {
  private:
    /// @brief Pointer to the currently opened scene.
    std::shared_ptr<Scene> m_Scene;

    /// @brief The entities to process the instruction on.
    std::vector<Entity> m_SourceEntities;

    /// @brief The entity to be the new parent of the source entities.
    entt::entity m_NewParent;

  public:
    /// @brief Executes the instruction.
    void
    PerformInstruction() override;
    NewParentInstruction(std::shared_ptr<Scene>     scene,
                         std::vector<Entity> const& sourceEntities,
                         entt::entity               newParent);
  };
}