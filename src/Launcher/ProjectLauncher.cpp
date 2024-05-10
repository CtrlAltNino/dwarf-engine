
#include "Launcher/ProjectLauncher.h"

#include "Launcher/ProjectLauncherModel.h"
#include "Launcher/ProjectLauncherView.h"

#include "Launcher/ProjectListHandler.h"
#include "Utilities/TimeUtilities.h"

namespace Dwarf
{
  ProjectLauncher::ProjectLauncher()
  {
    // Initializing the project launcher model (e.g. loading the project list)
    WindowProps props("Dwarf Engine", 1100, 600);

#if defined(_WIN32) || defined(__linux__)
    props.Api = GraphicsApi::OpenGL;
#endif

    m_Window = Window::Create(props);

    m_Model = std::make_unique<ProjectLauncherModel>();

    // Initializing the view
    m_View = std::make_unique<ProjectLauncherView>(m_Model, m_Window);
  }

  void
  ProjectLauncher::Run(std::filesystem::path& projectPath)
  {
    m_Window->ShowWindow();

    while (((m_Model->GetState() != ProjectChooserState::Done) &&
            (m_Model->GetState() != ProjectChooserState::Canceled)) &&
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

    int selectedProjectId = m_Model->GetSelectedProjectID();

    if (m_Model->GetState() == ProjectChooserState::Done)
    {
      ProjectInformation projectInformation =
        ProjectListHandler::GetProjectInformation(selectedProjectId);
      if (projectInformation.name != "")
      {
        projectPath = projectInformation.path;
        ProjectListHandler::RegisterProjectOpening(selectedProjectId);
      }
    }
  }
}