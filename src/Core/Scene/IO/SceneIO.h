#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Scene/ISceneFactory.h"
#include "Core/Scene/IO/ISceneIO.h"
#include "Core/Scene/IScene.h"
#include "Project/IProjectSettings.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include <filesystem>

namespace Dwarf
{
  class SceneIO : public ISceneIO
  {
  private:
    std::shared_ptr<ISceneFactory>    m_SceneFactory;
    std::shared_ptr<IProjectSettings> m_ProjectSettings;
    std::shared_ptr<IAssetDatabase>   m_AssetDatabase;
    std::shared_ptr<IFileHandler>     m_FileHandler;

    void
    WriteSceneToFile(const nlohmann::json&        serializedScene,
                     const std::filesystem::path& scenePath) const;

    std::string
    CreateNewSceneName(const std::filesystem::path& directory);

  public:
    SceneIO(std::shared_ptr<IProjectSettings> projectSettings,
            std::shared_ptr<ISceneFactory>    sceneFactory,
            std::shared_ptr<IAssetDatabase>   assetDatabase,
            std::shared_ptr<IFileHandler>     fileHandler);

    void
    SaveScene(IScene& scene) const override;

    void
    SaveSceneDialog(IScene& scene) const override;

    std::unique_ptr<IScene>
    LoadScene(IAssetReference& sceneAsset) const override;

    std::unique_ptr<IScene>
    LoadSceneDialog() const override;

    void
    NewSceneAsset(const std::filesystem::path& directory) override;

    // void
    // SetLastOpenedScene(AssetReference<SceneAsset> sceneAsset);
  };
}