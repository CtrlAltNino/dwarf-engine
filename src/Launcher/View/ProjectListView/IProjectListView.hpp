#pragma once

namespace Dwarf
{
  class IProjectListView
  {
  public:
    virtual void
    RenderProjectList(glm::ivec2 windowSize) = 0;
  };
}