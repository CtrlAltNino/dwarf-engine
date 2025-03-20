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

    void
    WriteSceneToFile(const nlohmann::json&        serializedScene,
                     const std::filesystem::path& scenePath) const;

    auto
    CreateNewSceneName(const std::filesystem::path& directory) -> std::string;

  public:
    SceneIO(std::shared_ptr<IDwarfLogger>     logger,
            std::shared_ptr<IProjectSettings> projectSettings,
            std::shared_ptr<ISceneFactory>    sceneFactory,
            std::shared_ptr<IAssetDatabase>   assetDatabase,
            std::shared_ptr<IFileHandler>     fileHandler);

    void
    SaveScene(IScene& scene) const override;

    void
    SaveSceneDialog(IScene& scene) const override;

    auto
    LoadScene(IAssetReference& sceneAsset) const
      -> std::unique_ptr<IScene> override;

    [[nodiscard]] auto
    LoadSceneDialog() const -> std::unique_ptr<IScene> override;

    void
    NewSceneAsset(const std::filesystem::path& directory) override;
  };
}