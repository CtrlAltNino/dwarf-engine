#include "Editor/Editor.h"

#include "Input/IInputManager.h"
#include "Utilities/TimeUtilities.h"

namespace Dwarf
{
  Editor::Editor(const std::shared_ptr<EditorLogger>&   logger,
                 const std::shared_ptr<IEditorStats>&   stats,
                 const std::shared_ptr<IInputManager>&  inputManager,
                 const std::shared_ptr<IAssetDatabase>& assetDatabase)
    : m_Logger(logger)
    , m_Stats(stats)
    , m_InputManager(inputManager)
    , m_AssetDatabase(assetDatabase)
  {
  }

  bool
  Editor::Run()
  {
    m_Stats->SetCurrentTimeStamp(TimeUtilities::GetCurrent());
    m_Stats->SetLastTimeStamp(TimeUtilities::GetCurrent());

    m_Logger->LogInfo("Starting editor loop");

    m_Window->ShowWindow();

    // TODO: abstract the close condition
    while (!m_Window->ShouldClose() && !m_Stats->GetCloseSignal())
    {
      // ===== Time related stuff
      m_Stats->SetLastTimeStamp(m_Stats->GetCurrentTimeStamp());
      m_Stats->SetCurrentTimeStamp(TimeUtilities::GetCurrent());

      m_Window->NewFrame();
      m_InputManager->OnUpdate();
      m_AssetDatabase->RecompileShaders();
      m_View->OnUpdate();
      m_View->OnImGuiRender();
      m_Window->EndFrame();

      // while (TimeUtilities::GetDifferenceInSeconds(
      //          TimeUtilities::GetCurrent(), m_Stats->GetCurrentTimeStamp()) <
      //        (1.0 / 60.0))
      // {
      //   // TODO: Update this when implementing multi threading
      // }
    }

    return m_Stats->GetReturnToLauncher();
  }
}