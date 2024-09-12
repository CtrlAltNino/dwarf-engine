#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Scene/IScene.h"

namespace Dwarf
{
  class ISceneIO
  {
  public:
    virtual ~ISceneIO() {}

    virtual void
    SaveScene(IScene& scene) const = 0;

    virtual void
    SaveSceneDialog(IScene& scene) const = 0;

    virtual std::unique_ptr<IScene>
    LoadScene(IAssetReference<SceneAsset>& sceneAsset) const = 0;

    virtual std::unique_ptr<IScene>
    LoadSceneDialog() const = 0;

    virtual std::unique_ptr<IScene>
    LoadDefaultScene() const = 0;
  };
}