#pragma once
#include "Core/Asset/AssetReimporter/IAssetReimporter.h"
#include "Core/Asset/Shader/IShaderRecompiler.h"
#include "Core/Asset/Texture/TextureWorker/ITextureLoadingWorker.h"
#include "Core/Rendering/DrawCall/DrawCallWorker/IDrawCallWorker.h"
#include "Core/Rendering/MeshBuffer/MeshBufferWorker/IMeshBufferWorker.h"
#include "Core/Scene/IO/ISceneIO.h"
#include "Core/Scene/ISceneFactory.h"
#include "Editor/IEditor.h"

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/EditorView/IEditorView.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Stats/IEditorStats.h"
#include "Input/IInputManager.h"
#include "Logging/IDwarfLogger.h"
#include "Project/IProjectSettings.h"
#include "Window/IWindow.h"
#include <memory>

namespace Dwarf
{
  class Editor : public IEditor
  {
  private:
    std::shared_ptr<IEditorView>           mView;
    std::shared_ptr<IWindow>               mWindow;
    std::shared_ptr<IDwarfLogger>          mLogger;
    std::shared_ptr<IEditorStats>          mEditorStats;
    std::shared_ptr<IInputManager>         mInputManager;
    std::shared_ptr<ILoadedScene>          mLoadedScene;
    std::shared_ptr<ISceneIO>              mSceneIO;
    std::shared_ptr<ISceneFactory>         mSceneFactory;
    std::shared_ptr<IProjectSettings>      mProjectSettings;
    std::shared_ptr<IAssetDatabase>        mAssetDatabase;
    std::shared_ptr<IShaderRecompiler>     mShaderRecompiler;
    std::shared_ptr<IAssetReimporter>      mAssetReimporter;
    std::shared_ptr<ITextureLoadingWorker> mTextureLoadingWorker;
    std::shared_ptr<IDrawCallWorker>       mDrawCallWorker;
    std::shared_ptr<IMeshBufferWorker>     mMeshBufferWorker;

  public:
    Editor(std::shared_ptr<IDwarfLogger>          logger,
           std::shared_ptr<IEditorStats>          stats,
           std::shared_ptr<IInputManager>         inputManager,
           std::shared_ptr<IProjectSettings>      projectSettings,
           std::shared_ptr<ILoadedScene>          loadedScene,
           std::shared_ptr<IWindow>               window,
           std::shared_ptr<ISceneIO>              sceneIO,
           std::shared_ptr<ISceneFactory>         sceneFactory,
           std::shared_ptr<IEditorView>           view,
           std::shared_ptr<IAssetDatabase>        assetDatabase,
           std::shared_ptr<IShaderRecompiler>     shaderRecompiler,
           std::shared_ptr<IAssetReimporter>      assetReimporter,
           std::shared_ptr<ITextureLoadingWorker> textureLoadingWorker,
           std::shared_ptr<IDrawCallWorker>       drawCallWorker,
           std::shared_ptr<IMeshBufferWorker>     meshBufferWorker);

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