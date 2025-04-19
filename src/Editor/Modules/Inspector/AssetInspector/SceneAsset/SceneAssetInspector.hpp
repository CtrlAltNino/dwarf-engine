#pragma once

#include "Core/Asset/AssetReimporter/IAssetReimporter.hpp"
#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Base.hpp"
#include "Core/Scene/IO/ISceneIO.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "Editor/Modules/Inspector/AssetInspector/SceneAsset/ISceneAssetInspector.hpp"
#include "Input/IInputManager.hpp"
#include "Utilities/FileHandler/IFileHandler.hpp"

namespace Dwarf
{
  class SceneAssetInspector : public ISceneAssetInspector
  {
  private:
    GraphicsApi                       mGraphicsApi;
    std::shared_ptr<IAssetDatabase>   mAssetDatabase;
    std::shared_ptr<IAssetReimporter> mAssetReimporter;
    std::shared_ptr<IInputManager>    mInputManager;
    std::shared_ptr<ISceneIO>         mSceneIO;
    std::shared_ptr<ILoadedScene>     mLoadedScene;
    std::shared_ptr<IFileHandler>     mFileHandler;

  public:
    SceneAssetInspector(GraphicsApi                       graphicsApi,
                        std::shared_ptr<IAssetDatabase>   assetDatabase,
                        std::shared_ptr<IAssetReimporter> assetReimporter,
                        std::shared_ptr<IInputManager>    inputManager,
                        std::shared_ptr<ISceneIO>         sceneIO,
                        std::shared_ptr<ILoadedScene>     loadedScene,
                        std::shared_ptr<IFileHandler>     fileHandler);
    ~SceneAssetInspector() override = default;

    /**
     * @brief Renders the scene asset inspector
     *
     * @param asset
     */
    void
    Render(IAssetReference& asset) override;
  };
}