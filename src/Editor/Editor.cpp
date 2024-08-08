#include "Editor/Editor.h"

#include "Input/IInputManager.h"
#include "Utilities/TimeUtilities.h"

namespace Dwarf
{
  Editor::Editor(const std::shared_ptr<IDwarfLogger>&   logger,
                 const std::shared_ptr<IEditorStats>&   stats,
                 const std::shared_ptr<IInputManager>&  inputManager,
                 const std::shared_ptr<IAssetDatabase>& assetDatabase,
                 const std::shared_ptr<IEditorView>&    view,
                 const std::shared_ptr<IWindow>&        window)
    : m_Logger(logger)
    , m_Stats(stats)
    , m_InputManager(inputManager)
    , m_AssetDatabase(assetDatabase)
    , m_View(view)
    , m_Window(window)
  {
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
    while (!m_Window->ShouldClose() && !m_Stats->GetCloseSignal())
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

    return m_Stats->GetReturnToLauncher();
  }
}