#include "Editor/Editor.h"

#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Scene/IScene.h"
#include "Input/IInputManager.h"
#include "Utilities/TimeUtilities.h"

namespace Dwarf
{
  Editor::Editor(std::shared_ptr<IDwarfLogger>          logger,
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
                 std::shared_ptr<IMeshBufferWorker>     meshBufferWorker)
    : m_Logger(logger)
    , m_EditorStats(stats)
    , m_InputManager(inputManager)
    , m_View(view)
    , m_Window(window)
    , m_SceneIO(sceneIO)
    , m_SceneFactory(sceneFactory)
    , m_ProjectSettings(projectSettings)
    , m_LoadedScene(loadedScene)
    , m_AssetDatabase(assetDatabase)
    , m_ShaderRecompiler(shaderRecompiler)
    , m_AssetReimporter(assetReimporter)
    , m_TextureLoadingWorker(textureLoadingWorker)
    , m_DrawCallWorker(drawCallWorker)
    , m_MeshBufferWorker(meshBufferWorker)
  {
    m_Logger->LogDebug(Log("Editor created", "Editor"));
  }

  Editor::~Editor()
  {
    m_Logger->LogDebug(Log("Editor destroyed", "Editor"));
  }

  bool
  Editor::Run()
  {
    m_EditorStats->SetCurrentTimeStamp(TimeUtilities::GetCurrent());
    m_EditorStats->SetLastTimeStamp(TimeUtilities::GetCurrent());

    m_Logger->LogInfo(Log("Starting the editor", "Editor"));

    m_AssetDatabase->ReimportAll();

    // Either load the last opened scene or the default scene
    if (m_ProjectSettings->GetLastOpenedScene().has_value() &&
        m_AssetDatabase->Exists(*m_ProjectSettings->GetLastOpenedScene()))
    {
      std::unique_ptr<IAssetReference> lastOpenedScene =
        m_AssetDatabase->Retrieve(*m_ProjectSettings->GetLastOpenedScene());

      std::unique_ptr<IScene> scene = m_SceneIO->LoadScene(*lastOpenedScene);
      m_LoadedScene->SetScene(std::move(scene));
    }
    else
    {
      std::unique_ptr<IScene> defaultScene =
        m_SceneFactory->CreateDefaultScene();
      m_LoadedScene->SetScene(std::move(defaultScene));
    }

    m_Logger->LogInfo(Log("Showing window", "Editor"));
    m_Window->ShowWindow();

    m_Logger->LogInfo(Log("Entering editor loop", "Editor"));
    m_Logger->LogWarn(
      Log("There is currently no fps cap implemented", "Editor"));

    m_DrawCallWorker->Invalidate();

    while (!m_Window->ShouldClose() && !m_EditorStats->GetCloseSignal())
    {
      //  ===== Time related stuff
      m_EditorStats->SetLastTimeStamp(m_EditorStats->GetCurrentTimeStamp());
      m_EditorStats->SetCurrentTimeStamp(TimeUtilities::GetCurrent());
      m_EditorStats->SetTimeSinceStart(m_EditorStats->GetTimeSinceStart() +
                                       m_EditorStats->GetDeltaTime());

      m_Window->NewFrame();
      m_InputManager->OnUpdate();
      m_AssetReimporter->ReimportQueuedAssets();
      m_ShaderRecompiler->Recompile();
      m_TextureLoadingWorker->ProcessTextureJobs();
      m_MeshBufferWorker->ProcessRequests();
      m_View->OnUpdate();
      m_View->OnImGuiRender();
      m_Window->EndFrame();

      /*while (TimeUtilities::GetDifferenceInSeconds(
               TimeUtilities::GetCurrent(),
               m_EditorStats->GetCurrentTimeStamp()) < (1.0 / 60.0))
      {
        // TODO: Update this when implementing multi threading
      }*/
    }

    m_LoadedScene->SetScene(nullptr);

    m_Logger->LogInfo(Log("Exiting editor loop", "Editor"));

    return m_EditorStats->GetReturnToLauncher();
  }
}