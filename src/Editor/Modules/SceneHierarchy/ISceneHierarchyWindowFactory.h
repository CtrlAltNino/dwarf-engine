#pragma once

#include "Editor/Modules/SceneHierarchy/SceneHierarchyWindow.h"

namespace Dwarf
{
  class ISceneHierarchyWindowFactory
  {
  public:
    virtual ~ISceneHierarchyWindowFactory() = default;
    virtual std::shared_ptr<SceneHierarchyWindow>
    Create() const = 0;

    virtual std::shared_ptr<SceneHierarchyWindow>
    Create(SerializedModule serializedModule) const = 0;
  };
} // namespace Dwarf