#pragma once

#include "Core/Asset/AssetReference/IAssetReference.hpp"
#include "Core/Scene/IScene.hpp"

namespace Dwarf
{
  /**
   * @brief Class that reads and writes scenes from disk
   *
   */
  class ISceneIO
  {
  public:
    virtual ~ISceneIO() = default;

    /**
     * @brief Serializes and writes a scene to it's designated path
     *
     * @param scene Scene to save to disk
     */
    virtual void
    SaveScene(IScene& scene) const = 0;

    /**
     * @brief Saves the scene after a path has been chosen by the user
     *
     * @param scene Scene to save to disk
     */
    virtual void
    SaveSceneDialog(IScene& scene) const = 0;

    /**
     * @brief Loads a scene from an asset file
     *
     * @param sceneAsset The scene asset file to load
     * @return Unique pointer to the loaded scene instance
     */
    [[nodiscard]] virtual auto
    LoadScene(IAssetReference& sceneAsset) const -> std::unique_ptr<IScene> = 0;

    /**
     * @brief Opens a dialog to chose a scene file from and loads it.
     *
     * @return Unique pointer to the loaded scene instance
     */
    [[nodiscard]] virtual auto
    LoadSceneDialog() const -> std::unique_ptr<IScene> = 0;

    /**
     * @brief Creates a new scene asset file in a given directory
     *
     * @param directory Directory in which the new scene asset file should be
     * written to
     */
    virtual void
    NewSceneAsset(const std::filesystem::path& directory) = 0;
  };
}