#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Scene/IScene.h"

namespace Dwarf
{
  class ISceneIO
  {
  public:
    virtual ~ISceneIO() = default;

    virtual void
    SaveScene(IScene& scene) const = 0;

    virtual void
    SaveSceneDialog(IScene& scene) const = 0;

    virtual auto
    LoadScene(IAssetReference& sceneAsset) const -> std::unique_ptr<IScene> = 0;

    [[nodiscard]] virtual auto
    LoadSceneDialog() const -> std::unique_ptr<IScene> = 0;

    /// @brief Creates a new empty scene.
    /// @param sceneProperties The properties of the scene.
    /// @return The created scene.
    virtual void
    NewSceneAsset(const std::filesystem::path& directory) = 0;
  };
}