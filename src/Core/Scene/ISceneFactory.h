#pragma once

#include "Core/Scene/IScene.h"

namespace Dwarf
{
  /// @brief Interface for a scene factory.
  class ISceneFactory
  {
  public:
    /// @brief Creates a scene from an asset reference.
    /// @param sceneAsset The asset reference of the scene.
    /// @return The created scene.
    virtual std::unique_ptr<IScene>
    FromAsset(IAssetReference<SceneAsset>& sceneAsset) = 0;

    virtual std::unique_ptr<IScene>
    CreateDefaultScene() = 0;
  };
} // namespace Dwarf