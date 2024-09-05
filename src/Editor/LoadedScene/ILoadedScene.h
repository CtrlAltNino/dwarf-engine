#pragma once

#include "Core/Scene/IScene.h"

namespace Dwarf
{
  class ILoadedScene
  {
  public:
    virtual ~ILoadedScene() = default;

    virtual std::shared_ptr<IScene>
    GetScene() const = 0;

    virtual void
    SetScene(std::shared_ptr<IScene> scene) = 0;
  };
}