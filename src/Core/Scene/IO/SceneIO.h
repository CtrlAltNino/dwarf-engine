#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Scene/ISceneFactory.h"
#include "Core/Scene/IO/ISceneIO.h"
#include "Core/Scene/IScene.h"
#include "Project/IProjectSettings.h"
#include <filesystem>

namespace Dwarf
{
  class SceneIO : public ISceneIO
  {
  private:
    std::shared_ptr<ISceneFactory>    m_SceneFactory;
    std::shared_ptr<IProjectSettings> m_ProjectSettings;
    std::shared_ptr<IAssetDatabase>   m_AssetDatabase;

    void
    WriteSceneToFile(nlohmann::json        serializedScene,
                     std::filesystem::path scenePath) const;

  public:
    SceneIO(std::shared_ptr<IProjectSettings> projectSettings,
            std::shared_ptr<ISceneFactory>    sceneFactory,
            std::shared_ptr<IAssetDatabase>   assetDatabase);

    void
    SaveScene(std::shared_ptr<IScene> scene) const override;

    void
    SaveSceneDialog(std::shared_ptr<IScene> scene) const override;

    std::shared_ptr<IScene>
    LoadScene(
      std::shared_ptr<AssetReference<SceneAsset>> sceneAsset) const override;

    std::shared_ptr<IScene>
    LoadSceneDialog() const override;

    std::shared_ptr<IScene>
    LoadDefaultScene() const override;

    // void
    // SetLastOpenedScene(AssetReference<SceneAsset> sceneAsset);
  };
}