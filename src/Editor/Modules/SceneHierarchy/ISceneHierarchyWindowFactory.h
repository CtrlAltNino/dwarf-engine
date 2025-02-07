#pragma once

#include "Editor/Modules/SceneHierarchy/SceneHierarchyWindow.h"

namespace Dwarf
{
  class ISceneHierarchyWindowFactory
  {
  public:
    virtual ~ISceneHierarchyWindowFactory() = default;
    virtual std::unique_ptr<SceneHierarchyWindow>
    Create() const = 0;

    virtual std::unique_ptr<SceneHierarchyWindow>
    Create(SerializedModule serializedModule) const = 0;
  };
} // namespace Dwarf