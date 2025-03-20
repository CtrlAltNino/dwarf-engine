#pragma once

#include "Core/Asset/AssetReimporter/IAssetReimporter.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Base.h"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.h"
#include "Core/Scene/IO/ISceneIO.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Modules/Inspector/AssetInspector/SceneAsset/ISceneAssetInspector.h"
#include "Input/IInputManager.h"
#include "Utilities/FileHandler/IFileHandler.h"

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

    void
    Render(IAssetReference& asset) override;
  };
}