#pragma once

#include "Core/Scene/Entity/Entity.h"
namespace Dwarf
{
  class IEntityInspector
  {
  public:
    virtual void
    Render(std::vector<Entity>&) = 0;
  };
}