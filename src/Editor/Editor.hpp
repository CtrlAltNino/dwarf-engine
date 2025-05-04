#pragma once

#include "Core/Asset/AssetReimporter/IAssetReimporter.hpp"
#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Asset/Shader/IShaderRecompiler.hpp"
#include "Core/Asset/Texture/TextureWorker/ITextureLoadingWorker.hpp"
#include "Core/Rendering/MeshBuffer/MeshBufferRequestList/IMeshBufferRequestList.hpp"
#include "Core/Scene/IO/ISceneIO.hpp"
#include "Core/Scene/ISceneFactory.hpp"
#include "Editor/EditorView/IEditorView.hpp"
#include "Editor/IEditor.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "Editor/Stats/IEditorStats.hpp"
#include "Input/IInputManager.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "Project/IProjectSettings.hpp"
#include "Window/IWindow.hpp"
#include <memory>

namespace Dwarf
{
  class Editor : public IEditor
  {
  private:
    std::shared_ptr<IEditorView>            mView;
    std::shared_ptr<IWindow>                mWindow;
    std::shared_ptr<IDwarfLogger>           mLogger;
    std::shared_ptr<IEditorStats>           mEditorStats;
    std::shared_ptr<IInputManager>          mInputManager;
    std::shared_ptr<ILoadedScene>           mLoadedScene;
    std::shared_ptr<ISceneIO>               mSceneIO;
    std::shared_ptr<ISceneFactory>          mSceneFactory;
    std::shared_ptr<IProjectSettings>       mProjectSettings;
    std::shared_ptr<IAssetDatabase>         mAssetDatabase;
    std::shared_ptr<IShaderRecompiler>      mShaderRecompiler;
    std::shared_ptr<IAssetReimporter>       mAssetReimporter;
    std::shared_ptr<ITextureLoadingWorker>  mTextureLoadingWorker;
    std::shared_ptr<IMeshBufferRequestList> mMeshBufferRequestList;

  public:
    Editor(std::shared_ptr<IDwarfLogger>           logger,
           std::shared_ptr<IEditorStats>           stats,
           std::shared_ptr<IInputManager>          inputManager,
           std::shared_ptr<IProjectSettings>       projectSettings,
           std::shared_ptr<ILoadedScene>           loadedScene,
           std::shared_ptr<IWindow>                window,
           std::shared_ptr<ISceneIO>               sceneIO,
           std::shared_ptr<ISceneFactory>          sceneFactory,
           std::shared_ptr<IEditorView>            view,
           std::shared_ptr<IAssetDatabase>         assetDatabase,
           std::shared_ptr<IShaderRecompiler>      shaderRecompiler,
           std::shared_ptr<IAssetReimporter>       assetReimporter,
           std::shared_ptr<ITextureLoadingWorker>  textureLoadingWorker,
           std::shared_ptr<IMeshBufferRequestList> MeshBufferRequestList);

    ~Editor() override;

    /*
     * @brief Starts the editor.
     * @return True if the user wants to return to the project launcher after
     * closing the editor.
     */
    auto
    Run() -> bool override;
  };
}