#pragma once

#include "Editor/Modules/SceneViewer/SceneViewerWindow.h"

namespace Dwarf
{
  class ISceneViewerWindowFactory
  {
  public:
    virtual ~ISceneViewerWindowFactory() = default;
    virtual std::shared_ptr<SceneViewerWindow>
    Create() const = 0;

    virtual std::shared_ptr<SceneViewerWindow>
    Create(SerializedModule serializedModule) const = 0;
  };
} // namespace Dwarf