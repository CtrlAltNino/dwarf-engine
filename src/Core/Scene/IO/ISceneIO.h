#pragma once

#include "Core/Asset/Database/AssetReference.h"
#include "Core/Scene/IScene.h"

namespace Dwarf
{
  class ISceneIO
  {
  public:
    virtual ~ISceneIO() {}

    virtual void
    SaveScene(std::shared_ptr<IScene> scene) const = 0;

    virtual void
    SaveSceneDialog(std::shared_ptr<IScene> scene) const = 0;

    virtual std::shared_ptr<IScene>
    LoadScene(std::shared_ptr<AssetReference<SceneAsset>> sceneAsset) const = 0;

    virtual std::shared_ptr<IScene>
    LoadSceneDialog() const = 0;

    virtual std::shared_ptr<IScene>
    LoadDefaultScene() const = 0;
  };
}