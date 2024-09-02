#include "Launcher/ProjectLauncher.h"
#include "Utilities/TimeUtilities.h"
#include "Core/Base.h"

namespace Dwarf
{
  ProjectLauncher::ProjectLauncher(std::shared_ptr<IWindow>              window,
                                   std::shared_ptr<IProjectLauncherView> view,
                                   std::shared_ptr<IDwarfLogger>         logger)
    : m_Window(window)
    , m_View(view)
    , m_Logger(logger)
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

  ProjectInformation
  ProjectLauncher::Run()
  {
    m_Logger->LogInfo(Log("Running project launcher...", "ProjectLauncher"));

    m_Logger->LogInfo(Log("Showing window...", "ProjectLauncher"));
    m_Window->ShowWindow();

    m_Logger->LogInfo(Log("Starting main loop...", "ProjectLauncher"));
    while (((m_State != ProjectChooserState::Done) &&
            (m_State != ProjectChooserState::Canceled)) &&
           !m_Window->ShouldClose())
    {
      TimeStamp currentTimeStamp = TimeUtilities::GetCurrent();

      m_Window->NewFrame();

      m_View->Render();

      m_Window->EndFrame();

      // TODO: Fps lock
      while (TimeUtilities::GetDifferenceInSeconds(
               TimeUtilities::GetCurrent(), currentTimeStamp) < (1.0 / 60.0))
      {
        // TODO: Update this when implementing multi threading
      }
    }

    m_Logger->LogInfo(Log("Project launcher finished", "ProjectLauncher"));
    return m_SelectedProject;
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