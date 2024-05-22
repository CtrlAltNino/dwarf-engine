#include "Editor/Editor.h"

#include "Core/Scene/SceneUtilities.h"
#include "Core/Asset/AssetDatabase.h"
#include "Input/IInputManager.h"
#include "Input/InputManager.h"
#include "Utilities/TimeUtilities.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Base.h"
// #include "Editor/EditorModel.h"
#include "Editor/EditorView.h"

namespace Dwarf
{
  Editor::Editor(const std::shared_ptr<EditorLogger>&  logger,
                 const std::shared_ptr<IEditorStats>&  stats,
                 const std::shared_ptr<IInputManager>& inputManager)
    : m_Logger(logger)
    , m_Stats(stats)
    , m_InputManager(inputManager)
  {
    // Init();
  }

  // void
  // Editor::UpdateWindowTitle() const
  // {
  //   std::string windowTitle = "Dwarf Engine Editor - ";
  //   windowTitle.append(m_Model->GetName());
  //   windowTitle.append(" - ");
  //   windowTitle.append(m_Model->GetScene()->GetName());
  //   windowTitle.append(" <");
  //   windowTitle.append(graphicsApiNames[(int)m_Window->GetApi()]);
  //   windowTitle.append(">");

  //   m_Logger->LogInfo("[EDITOR] Updating window title");

  //   m_Window->SetWindowTitle(windowTitle);
  // }

  void
  Editor::Init()
  {
    // m_Logger->LogInfo("Initializing dwarf engine editor for project at [" +
    //                   m_Model->GetProjectPath().string() + "]");

    // WindowProps props("Dwarf Engine Editor", 1280, 720);

    // props.Api = (GraphicsApi)projectSettings["graphicsApi"];

    // TODO: Create error popup for invalid project

    // ========== Create window ==========
    // std::cout << "[EDITOR INIT] Creating editor window" << std::endl;
    // m_Window = Window::Create(props);

    // // ========== Create renderer ==========
    // Renderer::Create(props.Api, Renderer::RendererType::Forward);

    // // ========== Initialize Asset Database ==========
    // std::cout << "[EDITOR INIT] Loading asset database" << std::endl;
    // AssetDatabase::Init(m_Model->GetProjectPath());

    // if (projectSettings.contains("lastOpenedScene"))
    // {
    //   std::shared_ptr<AssetReference<SceneAsset>> lastOpenedSceneAsset =
    //     AssetDatabase::Retrieve<SceneAsset>(
    //       std::make_shared<UID>(projectSettings["lastOpenedScene"]));
    //   if (lastOpenedSceneAsset)
    //   {
    //     std::cout << "[EDITOR INIT] Loading last opened scene" << std::endl;
    //     m_Model->SetScene(
    //       SceneUtilities::LoadScene(lastOpenedSceneAsset->GetAsset()->m_Path));
    //   }
    //   else
    //   {
    //     std::cout
    //       << "[EDITOR INIT] No last opened scene found. Loading default
    //       scene"
    //       << std::endl;
    //     m_Model->SetScene(SceneUtilities::LoadDefaultScene());
    //   }
    // }
    // else
    // {
    //   std::cout
    //     << "[EDITOR INIT] No last opened scene found. Loading default scene"
    //     << std::endl;
    //   m_Model->SetScene(SceneUtilities::LoadDefaultScene());
    // }

    // // ========== Initialize Editor view ==========
    // m_View = std::make_unique<EditorView>(m_Model);

    // Get monitor variables

    // Initialize view
    // std::cout << "[EDITOR INIT] Setting up view" << std::endl;

    // // ========== Set window title ======
    // UpdateWindowTitle();

    // // ========== Set window icon ==========

    // // ========== Set window size ==========

    // // ========== Set window to be maximized ==========
    // m_Window->MaximizeWindow();
    // // m_View->Init();

    // // ========== Set window size constraints ==========

    // // ========== Show window ==========
    // std::cout << "[EDITOR INIT] Making window visible" << std::endl;
    // m_Window->ShowWindow();
    // std::cout << "[EDITOR INIT] Editor initialization done" << std::endl;
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
      AssetDatabase::RecompileShaders();
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