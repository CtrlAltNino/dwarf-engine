#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Scene/IO/ISceneIO.h"
#include "Core/Scene/IScene.h"
#include "Core/Scene/ISceneFactory.h"
#include "Logging/IDwarfLogger.h"
#include "Project/IProjectSettings.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include <filesystem>

namespace Dwarf
{
  class SceneIO : public ISceneIO
  {
  private:
    std::shared_ptr<IDwarfLogger>     mLogger;
    std::shared_ptr<ISceneFactory>    mSceneFactory;
    std::shared_ptr<IProjectSettings> mProjectSettings;
    std::shared_ptr<IAssetDatabase>   mAssetDatabase;
    std::shared_ptr<IFileHandler>     mFileHandler;

    /**
     * @brief Writes the serialized scene data to a given path
     *
     * @param serializedScene Serialized scene structure
     * @param scenePath Path to write the scene to
     */
    void
    WriteSceneToFile(const nlohmann::json&        serializedScene,
                     const std::filesystem::path& scenePath) const;

    /**
     * @brief Helper class creating the next free default scene name in a given
     * directory
     *
     * @param directory Directory in which a free name should be determined
     * @return Created scene name
     */
    auto
    CreateNewSceneName(const std::filesystem::path& directory) -> std::string;

  public:
    SceneIO(std::shared_ptr<IDwarfLogger>     logger,
            std::shared_ptr<IProjectSettings> projectSettings,
            std::shared_ptr<ISceneFactory>    sceneFactory,
            std::shared_ptr<IAssetDatabase>   assetDatabase,
            std::shared_ptr<IFileHandler>     fileHandler);

    /**
     * @brief Serializes and writes a scene to it's designated path
     *
     * @param scene Scene to save to disk
     */
    void
    SaveScene(IScene& scene) const override;

    /**
     * @brief Saves the scene after a path has been chosen by the user
     *
     * @param scene Scene to save to disk
     */
    void
    SaveSceneDialog(IScene& scene) const override;

    /**
     * @brief Loads a scene from an asset file
     *
     * @param sceneAsset The scene asset file to load
     * @return Unique pointer to the loaded scene instance
     */
    auto
    LoadScene(IAssetReference& sceneAsset) const
      -> std::unique_ptr<IScene> override;

    /**
     * @brief Opens a dialog to chose a scene file from and loads it.
     *
     * @return Unique pointer to the loaded scene instance
     */
    [[nodiscard]] auto
    LoadSceneDialog() const -> std::unique_ptr<IScene> override;

    /**
     * @brief Creates a new scene asset file in a given directory
     *
     * @param directory Directory in which the new scene asset file should be
     * written to
     */
    void
    NewSceneAsset(const std::filesystem::path& directory) override;
  };
}