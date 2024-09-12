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

    /// @brief Creates a new empty scene.
    /// @param sceneProperties The properties of the scene.
    /// @return The created scene.
    virtual std::unique_ptr<IScene>
    NewSceneAsset(const std::filesystem::path& directory) = 0;

    virtual std::unique_ptr<IScene>
    NewEmptyScene() = 0;
  };
} // namespace Dwarf