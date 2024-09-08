#include "Launcher/ProjectLauncher.h"
#include "Utilities/TimeUtilities.h"
#include "Core/Base.h"

namespace Dwarf
{
  ProjectLauncher::ProjectLauncher(IDwarfLogger&                         logger,
                                   std::unique_ptr<IProjectLauncherView> view,
                                   IProjectLauncherData&                 data,
                                   IProjectList& projectList)
    : m_View(std::move(view))
    , m_Data(data)
    , m_Logger(logger)
    , m_ProjectList(projectList)
  {
    m_Logger.LogInfo(Log("Creating ProjectLauncher", "ProjectLauncher"));
  }

  ProjectLauncher::~ProjectLauncher()
  {
    m_Logger.LogInfo(Log("Destroying ProjectLauncher", "ProjectLauncher"));
  }

  ProjectInformation
  ProjectLauncher::Run()
  {
    m_Logger.LogInfo(Log("Running project launcher...", "ProjectLauncher"));

    m_Logger.LogInfo(Log("Showing window...", "ProjectLauncher"));
    // m_Window->ShowWindow();
    m_View->Show();

    m_Logger.LogInfo(Log("Starting main loop...", "ProjectLauncher"));
    while ((m_Data.GetState() != ProjectChooserState::Done) &&
           (m_Data.GetState() != ProjectChooserState::Cancelled))
    {
      TimeStamp currentTimeStamp = TimeUtilities::GetCurrent();

      // m_Window->NewFrame();

      m_View->Render();
      // m_Data.SetState(ProjectChooserState::Done);

      // m_Window->EndFrame();

      // TODO: Fps lock
      while (TimeUtilities::GetDifferenceInSeconds(
               TimeUtilities::GetCurrent(), currentTimeStamp) < (1.0 / 60.0))
      {
        // TODO: Update this when implementing multi threading
      }
    }

    m_Logger.LogInfo(Log("Project launcher finished", "ProjectLauncher"));
    if (m_Data.GetState() == ProjectChooserState::Done)
    {
      m_ProjectList.RegisterProjectOpening(m_Data.GetSelectedProject().path);
    }
    return m_Data.GetSelectedProject();
  }
}