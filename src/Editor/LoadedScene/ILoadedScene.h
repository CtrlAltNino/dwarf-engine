#pragma once

#include "Core/Scene/IScene.h"

namespace Dwarf
{
  class ILoadedScene
  {
  public:
    virtual ~ILoadedScene() = default;

    virtual IScene&
    GetScene() = 0;

    virtual void
    SetScene(std::unique_ptr<IScene> scene) = 0;
  };
}