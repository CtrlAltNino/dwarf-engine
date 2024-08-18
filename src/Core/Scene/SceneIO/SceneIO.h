#pragma once

#include "Core/Scene/IScene.h"
#include "Utilities/FileHandler.h"

namespace Dwarf
{
  class SceneIO
  {
  public:
    void
    SaveScene(std::shared_ptr<IScene> scene);

    void
    SaveSceneDialog(std::shared_ptr<IScene> scene);

    std::shared_ptr<IScene>
    LoadScene(AssetReference<SceneAsset> sceneAsset);

    std::shared_ptr<IScene>
    LoadSceneDialog();

    std::shared_ptr<IScene>
    LoadDefaultScene();

    void
    SetLastOpenedScene(AssetReference<SceneAsset> sceneAsset);
  };
}