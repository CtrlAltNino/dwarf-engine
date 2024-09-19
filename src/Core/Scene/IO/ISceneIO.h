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
    LoadScene(IAssetReference& sceneAsset) const = 0;

    virtual std::unique_ptr<IScene>
    LoadSceneDialog() const = 0;

    /// @brief Creates a new empty scene.
    /// @param sceneProperties The properties of the scene.
    /// @return The created scene.
    virtual void
    NewSceneAsset(const std::filesystem::path& directory) = 0;
  };
}