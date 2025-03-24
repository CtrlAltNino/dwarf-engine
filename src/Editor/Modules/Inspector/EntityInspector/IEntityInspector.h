#pragma once
#include <entt/entt.hpp>

namespace Dwarf
{
  /**
   * @brief Class that renders the inspector for scene entities
   *
   */
  class IEntityInspector
  {
  public:
    virtual ~IEntityInspector() = default;

    /**
     * @brief Renders the scene entity inspector
     *
     */
    virtual void
    Render(const std::vector<entt::entity>& entities) = 0;
  };
}