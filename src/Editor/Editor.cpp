#include "Editor/Editor.h"

#include "Input/IInputManager.h"
#include "Utilities/TimeUtilities.h"

namespace Dwarf
{
  Editor::Editor(std::shared_ptr<IDwarfLogger>     logger,
                 std::shared_ptr<IEditorStats>     stats,
                 std::shared_ptr<IInputManager>    inputManager,
                 std::shared_ptr<IAssetDatabase>   assetDatabase,
                 std::shared_ptr<IEditorView>      view,
                 std::shared_ptr<IWindow>          window,
                 std::shared_ptr<ISceneIO>         sceneIO,
                 std::shared_ptr<IProjectSettings> projectSettings)
    : m_Logger(logger)
    , m_Stats(stats)
    , m_InputManager(inputManager)
    , m_AssetDatabase(assetDatabase)
    , m_View(view)
    , m_Window(window)
    , m_SceneIO(sceneIO)
    , m_ProjectSettings(projectSettings)
  {
    // Either load the last opened scene or the default scene
    if (m_ProjectSettings->GetLastOpenedScene() != nullptr)
    {
      m_Scene = m_SceneIO->LoadScene(m_AssetDatabase->Retrieve<SceneAsset>(
        m_ProjectSettings->GetLastOpenedScene()));
    }
    else
    {
      m_Scene = m_SceneIO->LoadDefaultScene();
    }
  }

  bool
  Editor::Run()
  {
    m_Stats->SetCurrentTimeStamp(TimeUtilities::GetCurrent());
    m_Stats->SetLastTimeStamp(TimeUtilities::GetCurrent());

    m_Logger->LogInfo(Log("Starting the editor", "Editor"));

    m_Logger->LogInfo(Log("Showing window", "Editor"));
    m_Window->ShowWindow();

    m_Logger->LogInfo(Log("Entering editor loop", "Editor"));
    m_Logger->LogWarn(
      Log("There is currently no fps cap implemented", "Editor"));
    // TODO: abstract the close condition
    while (!m_Window->ShouldClose() && !m_CloseSignal)
    {
      m_Logger->LogInfo(Log("Handling time stamps", "Editor"));
      // ===== Time related stuff
      m_Stats->SetLastTimeStamp(m_Stats->GetCurrentTimeStamp());
      m_Stats->SetCurrentTimeStamp(TimeUtilities::GetCurrent());

      m_Window->NewFrame();
      m_InputManager->OnUpdate();
      // m_AssetDatabase->RecompileShaders();
      m_View->OnUpdate();
      m_View->OnImGuiRender();
      m_Window->EndFrame();

      m_Logger->LogInfo(Log("Editor loop end", "Editor"));
      // while (TimeUtilities::GetDifferenceInSeconds(
      //          TimeUtilities::GetCurrent(), m_Stats->GetCurrentTimeStamp()) <
      //        (1.0 / 60.0))
      // {
      //   // TODO: Update this when implementing multi threading
      // }
    }

    m_Logger->LogInfo(Log("Exiting editor loop", "Editor"));

    return m_ReturnToLauncher;
  }

  void
  Editor::SetReturnToLauncher(bool returnToLauncher)
  {
    m_ReturnToLauncher = returnToLauncher;
  }

  bool
  Editor::GetReturnToLauncher() const
  {
    return m_ReturnToLauncher;
  }

  void
  Editor::SetCloseSignal(bool closeSignal)
  {
    m_CloseSignal = closeSignal;
  }

  bool
  Editor::GetCloseSignal() const
  {
    return m_CloseSignal;
  }

  void
  Editor::SetScene(std::shared_ptr<IScene> scene)
  {
    m_Scene = scene;
  }

  std::shared_ptr<IScene>
  Editor::GetScene() const
  {
    return m_Scene;
  }
}