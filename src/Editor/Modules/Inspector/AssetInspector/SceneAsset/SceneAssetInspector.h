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
    GraphicsApi                       m_GraphicsApi;
    std::shared_ptr<IAssetDatabase>   m_AssetDatabase;
    std::shared_ptr<IAssetReimporter> m_AssetReimporter;
    std::shared_ptr<IInputManager>    m_InputManager;
    std::shared_ptr<ISceneIO>         m_SceneIO;
    std::shared_ptr<ILoadedScene>     m_LoadedScene;
    std::shared_ptr<IFileHandler>     m_FileHandler;

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