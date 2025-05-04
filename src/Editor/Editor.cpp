#include "pch.hpp"

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Scene/IScene.hpp"
#include "Editor/Editor.hpp"
#include "Input/IInputManager.hpp"
#include "Utilities/TimeUtilities.hpp"

namespace Dwarf
{
  Editor::Editor(std::shared_ptr<IDwarfLogger>           logger,
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
                 std::shared_ptr<IMeshBufferRequestList> MeshBufferRequestList)
    : mLogger(std::move(logger))
    , mEditorStats(std::move(stats))
    , mInputManager(std::move(inputManager))
    , mView(std::move(view))
    , mWindow(std::move(window))
    , mSceneIO(std::move(sceneIO))
    , mSceneFactory(std::move(sceneFactory))
    , mProjectSettings(std::move(projectSettings))
    , mLoadedScene(std::move(loadedScene))
    , mAssetDatabase(std::move(assetDatabase))
    , mShaderRecompiler(std::move(shaderRecompiler))
    , mAssetReimporter(std::move(assetReimporter))
    , mTextureLoadingWorker(std::move(textureLoadingWorker))
    , mMeshBufferRequestList(std::move(MeshBufferRequestList))
  {
    mLogger->LogDebug(Log("Editor created", "Editor"));
  }

  Editor::~Editor()
  {
    mLogger->LogDebug(Log("Editor destroyed", "Editor"));
  }

  bool
  Editor::Run()
  {
    mEditorStats->SetCurrentTimeStamp(TimeUtilities::GetCurrent());
    mEditorStats->SetLastTimeStamp(TimeUtilities::GetCurrent());

    mLogger->LogInfo(Log("Starting the editor", "Editor"));

    mAssetDatabase->ReimportAll();

    // Either load the last opened scene or the default scene
    if (mProjectSettings->GetLastOpenedScene().has_value() &&
        mAssetDatabase->Exists(*mProjectSettings->GetLastOpenedScene()))
    {
      std::unique_ptr<IAssetReference> lastOpenedScene =
        mAssetDatabase->Retrieve(*mProjectSettings->GetLastOpenedScene());

      std::unique_ptr<IScene> scene = mSceneIO->LoadScene(*lastOpenedScene);
      mLoadedScene->SetScene(std::move(scene));
    }
    else
    {
      std::unique_ptr<IScene> defaultScene =
        mSceneFactory->CreateDefaultScene();
      mLoadedScene->SetScene(std::move(defaultScene));
    }

    mLogger->LogInfo(Log("Showing window", "Editor"));
    mWindow->ShowWindow();

    mLogger->LogInfo(Log("Entering editor loop", "Editor"));
    mLogger->LogWarn(
      Log("There is currently no fps cap implemented", "Editor"));

    mEditorStats->SetInitialTimeStamp(TimeUtilities::GetCurrent());

    while (!mWindow->ShouldClose() && !mEditorStats->GetCloseSignal())
    {
      //  ===== Time related stuff
      mEditorStats->SetLastTimeStamp(mEditorStats->GetCurrentTimeStamp());
      mEditorStats->SetCurrentTimeStamp(TimeUtilities::GetCurrent());

      mWindow->NewFrame();
      mInputManager->OnUpdate();
      mAssetReimporter->ReimportQueuedAssets();
      mShaderRecompiler->Recompile();
      mTextureLoadingWorker->ProcessTextureJobs();
      mMeshBufferRequestList->ProcessRequests();
      mView->OnUpdate();
      mView->OnImGuiRender();
      mWindow->EndFrame();

      /*while (TimeUtilities::GetDifferenceInSeconds(
               TimeUtilities::GetCurrent(),
               mEditorStats->GetCurrentTimeStamp()) < (1.0 / 60.0))
      {
        // TODO: Update this when implementing multi threading
      }*/
    }

    mLogger->LogInfo(Log("Exiting editor loop", "Editor"));

    return mEditorStats->GetReturnToLauncher();
  }
}