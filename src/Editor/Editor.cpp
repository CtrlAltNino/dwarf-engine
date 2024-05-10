#include "Editor/Editor.h"

#include "Core/Scene/SceneUtilities.h"
#include "Core/Asset/AssetDatabase.h"
#include "Input/InputManager.h"
#include "Utilities/TimeUtilities.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Base.h"

namespace Dwarf
{

  std::shared_ptr<Editor> Editor::s_Instance = nullptr;

  std::shared_ptr<Editor>
  CreateEditor()
  {
    return std::make_shared<Editor>();
  }

  Editor::Editor()
  {
    s_Instance = std::make_shared<Editor>(*this);
  }

  Editor::~Editor() = default;

  void
  Editor::UpdateWindowTitle() const
  {
    std::string windowTitle = "Dwarf Engine Editor - ";
    windowTitle.append(s_Instance->m_Model->GetName());
    windowTitle.append(" - ");
    windowTitle.append(s_Instance->m_Model->GetScene()->GetName());
    windowTitle.append(" <");
    windowTitle.append(graphicsApiNames[(int)s_Instance->m_Window->GetApi()]);
    windowTitle.append(">");

    std::cout << "[EDITOR] Updating window title" << std::endl;

    s_Instance->m_Window->SetWindowTitle(windowTitle);
  }

  void
  Editor::Init(std::filesystem::path const& projectPath)
  {
    // s_Instance = this;

    // ========== Load .dproj file ==========
    std::cout
      << "[EDITOR INIT] Initializing dwarf engine editor for project at ["
      << projectPath << "]" << std::endl;
    std::filesystem::path projectSettingsPath = projectPath;
    projectSettingsPath.append("projectSettings.dproj");
    std::cout << "[EDITOR INIT] Loading .dproj project file at ["
              << projectSettingsPath << "]" << std::endl;
    nlohmann::json projectSettings = nlohmann::json::parse(
      FileHandler::ReadFile(projectSettingsPath))["projectInformation"];

    WindowProps props("Dwarf Engine Editor", 1280, 720);

    props.Api = (GraphicsApi)projectSettings["graphicsApi"];

    // TODO: Create error popup for invalid project

    // ========== Create window ==========
    std::cout << "[EDITOR INIT] Creating editor window" << std::endl;
    s_Instance->m_Window = Window::Create(props);

    // ========== Create renderer ==========
    Renderer::Create(props.Api, Renderer::RendererType::Forward);

    // ========== Initialize Asset Database ==========
    std::cout << "[EDITOR INIT] Loading asset database" << std::endl;
    AssetDatabase::Init(projectPath);

    // ========== Initialize Editor model
    std::cout << "[EDITOR INIT] Initializing editor model" << std::endl;

    s_Instance->m_Model = std::make_shared<EditorModel>(
      projectSettings["projectName"].get<std::string_view>(), projectPath);

    if (projectSettings.contains("lastOpenedScene"))
    {
      std::shared_ptr<AssetReference<SceneAsset>> lastOpenedSceneAsset =
        AssetDatabase::Retrieve<SceneAsset>(
          std::make_shared<UID>(projectSettings["lastOpenedScene"]));
      if (lastOpenedSceneAsset)
      {
        std::cout << "[EDITOR INIT] Loading last opened scene" << std::endl;
        s_Instance->m_Model->SetScene(
          SceneUtilities::LoadScene(lastOpenedSceneAsset->GetAsset()->m_Path));
      }
      else
      {
        std::cout
          << "[EDITOR INIT] No last opened scene found. Loading default scene"
          << std::endl;
        s_Instance->m_Model->SetScene(SceneUtilities::LoadDefaultScene());
      }
    }
    else
    {
      std::cout
        << "[EDITOR INIT] No last opened scene found. Loading default scene"
        << std::endl;
      s_Instance->m_Model->SetScene(SceneUtilities::LoadDefaultScene());
    }

    // ========== Initialize Editor view ==========
    s_Instance->m_View = std::make_shared<EditorView>(s_Instance->m_Model);

    // Get monitor variables

    // Initialize view
    std::cout << "[EDITOR INIT] Setting up view" << std::endl;

    // ========== Set window title ======
    UpdateWindowTitle();

    // ========== Set window icon ==========

    // ========== Set window size ==========

    // ========== Set window to be maximized ==========
    s_Instance->m_Window->MaximizeWindow();
    s_Instance->m_View->Init();

    // ========== Set window size constraints ==========

    // ========== Show window ==========
    std::cout << "[EDITOR INIT] Making window visible" << std::endl;
    s_Instance->m_Window->ShowWindow();
    std::cout << "[EDITOR INIT] Editor initialization done" << std::endl;
  }

  bool
  Editor::Run(std::filesystem::path const& projectPath)
  {
    Init(projectPath);

    TimeStamp currentFrameStamp = TimeUtilities::GetCurrent();
    TimeStamp lastFrameStamp = TimeUtilities::GetCurrent();

    // TODO: abstract the close condition
    while (!s_Instance->m_Window->ShouldClose() &&
           !s_Instance->m_Model->GetCloseSignal())
    {
      // ===== Time related stuff
      lastFrameStamp = currentFrameStamp;
      currentFrameStamp = TimeUtilities::GetCurrent();
      s_Instance->m_Model->SetDeltaTime(TimeUtilities::GetDifferenceInSeconds(
        currentFrameStamp, lastFrameStamp));

      s_Instance->m_Window->NewFrame();
      InputManager::OnUpdate();
      AssetDatabase::RecompileShaders();
      s_Instance->m_View->OnUpdate(s_Instance->m_Model->GetDeltaTime());
      s_Instance->m_View->OnImGuiRender();
      s_Instance->m_Window->EndFrame();

      while (TimeUtilities::GetDifferenceInSeconds(
               TimeUtilities::GetCurrent(), currentFrameStamp) < (1.0 / 60.0))
      {
        // TODO: Update this when implementing multi threading
      }
    }

    return s_Instance->m_Model->GetReturnToLauncher();
  }
}