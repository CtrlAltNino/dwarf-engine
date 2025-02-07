#pragma once

namespace Dwarf
{
  class IButtonsView
  {
  public:
    virtual void
    RenderButtons(glm::ivec2 windowSize) = 0;
  };
}