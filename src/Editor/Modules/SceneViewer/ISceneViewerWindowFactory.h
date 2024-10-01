#pragma once

#include "Editor/Modules/SceneViewer/SceneViewerWindow.h"

namespace Dwarf
{
  class ISceneViewerWindowFactory
  {
  public:
    virtual ~ISceneViewerWindowFactory() = default;
    virtual std::unique_ptr<SceneViewerWindow>
    Create() const = 0;

    virtual std::unique_ptr<SceneViewerWindow>
    Create(SerializedModule serializedModule) const = 0;
  };
}