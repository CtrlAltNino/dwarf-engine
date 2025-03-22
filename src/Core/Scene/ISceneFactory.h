#pragma once

#include "Core/Scene/IScene.h"

namespace Dwarf
{
  /// @brief Interface for a scene factory.
  class ISceneFactory
  {
  public:
    virtual ~ISceneFactory() = default;
    /// @brief Creates a scene from an asset reference.
    /// @param sceneAsset The asset reference of the scene.
    /// @return The created scene.
    [[nodiscard]] virtual auto
    FromAsset(IAssetReference& sceneAsset) const -> std::unique_ptr<IScene> = 0;

    /**
     * @brief Creates a default scene
     *
     * @return Unique pointer to the created scene instance
     */
    [[nodiscard]] virtual auto
    CreateDefaultScene() const -> std::unique_ptr<IScene> = 0;
  };
} // namespace Dwarf