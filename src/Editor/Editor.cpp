#include "Editor/Editor.h"

#include "Input/IInputManager.h"
#include "Utilities/TimeUtilities.h"

namespace Dwarf
{
  Editor::Editor(std::shared_ptr<IDwarfLogger>      logger,
                 std::shared_ptr<IEditorStats>      stats,
                 std::shared_ptr<IInputManager>     inputManager,
                 std::shared_ptr<IProjectSettings>  projectSettings,
                 std::shared_ptr<ILoadedScene>      loadedScene,
                 std::shared_ptr<IWindow>           window,
                 std::shared_ptr<ISceneIO>          sceneIO,
                 std::shared_ptr<IEditorView>       view,
                 std::shared_ptr<IAssetDatabase>    assetDatabase,
                 std::shared_ptr<IShaderRecompiler> shaderRecompiler)
    : m_Logger(logger)
    , m_EditorStats(stats)
    , m_InputManager(inputManager)
    , m_View(view)
    , m_Window(window)
    , m_SceneIO(sceneIO)
    , m_ProjectSettings(projectSettings)
    , m_LoadedScene(loadedScene)
    , m_AssetDatabase(assetDatabase)
    , m_ShaderRecompiler(shaderRecompiler)

  {
    // Either load the last opened scene or the default scene
    if (m_ProjectSettings->GetLastOpenedScene() != nullptr)
    {
      m_LoadedScene->SetScene(
        m_SceneIO->LoadScene(m_AssetDatabase->Retrieve<SceneAsset>(
          m_ProjectSettings->GetLastOpenedScene())));
    }
    else
    {
      m_LoadedScene->SetScene(m_SceneIO->LoadDefaultScene());
    }
  }

  bool
  Editor::Run()
  {
    m_EditorStats->SetCurrentTimeStamp(TimeUtilities::GetCurrent());
    m_EditorStats->SetLastTimeStamp(TimeUtilities::GetCurrent());

    m_Logger->LogInfo(Log("Starting the editor", "Editor"));

    m_Logger->LogInfo(Log("Showing window", "Editor"));
    m_Window->ShowWindow();

    m_Logger->LogInfo(Log("Entering editor loop", "Editor"));
    m_Logger->LogWarn(
      Log("There is currently no fps cap implemented", "Editor"));

    while (!m_Window->ShouldClose() && !m_CloseSignal)
    {
      // m_Logger->LogInfo(Log("Handling time stamps", "Editor"));
      //  ===== Time related stuff
      m_EditorStats->SetLastTimeStamp(m_EditorStats->GetCurrentTimeStamp());
      m_EditorStats->SetCurrentTimeStamp(TimeUtilities::GetCurrent());

      m_Window->NewFrame();
      m_InputManager->OnUpdate();
      m_ShaderRecompiler->Recompile();
      m_View->OnUpdate();
      m_View->OnImGuiRender();
      m_Window->EndFrame();

      // m_Logger->LogInfo(Log("Editor loop end", "Editor"));
      //  while (TimeUtilities::GetDifferenceInSeconds(
      //           TimeUtilities::GetCurrent(), m_Stats->GetCurrentTimeStamp())
      //           <
      //         (1.0 / 60.0))
      //  {
      //    // TODO: Update this when implementing multi threading
      //  }
    }

    m_Logger->LogInfo(Log("Exiting editor loop", "Editor"));

    return m_ReturnToLauncher;
  }
}