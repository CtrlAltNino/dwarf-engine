#include "Launcher/ProjectLauncher.h"
#include "Utilities/TimeUtilities.h"
#include "Core/Base.h"

namespace Dwarf
{
  ProjectLauncher::ProjectLauncher(std::shared_ptr<IWindow>              window,
                                   std::shared_ptr<IProjectLauncherView> view)
    : m_Window(window)
    , m_View(view)
  {
    //     // Initializing the project launcher model (e.g. loading the project
    //     list) WindowProps props("Dwarf Engine", 1100, 600);

    // #if defined(_WIN32) || defined(__linux__)
    //     props.Api = GraphicsApi::OpenGL;
    // #endif

    //     m_Window = Window::Create(props);

    //     m_Model = std::make_unique<ProjectLauncherModel>();

    //     // Initializing the view
    //     m_View = std::make_unique<ProjectLauncherView>(m_Model, m_Window);
  }

  ProjectPath
  ProjectLauncher::Run()
  {
    m_Window->ShowWindow();

    while (((m_State != ProjectChooserState::Done) &&
            (m_State != ProjectChooserState::Canceled)) &&
           !m_Window->ShouldClose())
    {
      TimeStamp currentFrameStamp = TimeUtilities::GetCurrent();

      m_Window->NewFrame();

      m_View->Render();

      m_Window->EndFrame();

      // TODO: Fps lock
      while (TimeUtilities::GetDifferenceInSeconds(
               TimeUtilities::GetCurrent(), currentFrameStamp) < (1.0 / 60.0))
      {
        // TODO: Update this when implementing multi threading
      }
    }

    // int selectedProjectId = m_Model->GetSelectedProjectID();

    // if (m_State == ProjectChooserState::Done)
    // {
    //   ProjectInformation projectInformation =
    //     ProjectListHandler::GetProjectInformation(selectedProjectId);
    //   if (projectInformation.name != "")
    //   {
    //     projectPath = projectInformation.path;
    //     ProjectListHandler::RegisterProjectOpening(selectedProjectId);
    //   }
    // }
  }

  ProjectChooserState
  ProjectLauncher::GetState() const
  {
    return m_State;
  }

  void
  ProjectLauncher::SetState(ProjectChooserState state)
  {
    m_State = state;
  }

  void
  ProjectLauncher::SetSelectedProject(const ProjectInformation& project)
  {
    m_SelectedProject = project;
  }

  const ProjectInformation&
  ProjectLauncher::GetSelectedProject() const
  {
    return m_SelectedProject;
  }
}