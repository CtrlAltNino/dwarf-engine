#pragma once
#include <entt/entt.hpp>

namespace Dwarf
{
  class IEntityInspector
  {
  public:
    virtual ~IEntityInspector() = default;

    virtual void
    Render(std::vector<entt::entity>&) = 0;
  };
}