#pragma once

namespace Dwarf
{
  class IFooterView
  {
  public:
    virtual void
    RenderFooter(glm::ivec2 windowSize) = 0;
  };
}