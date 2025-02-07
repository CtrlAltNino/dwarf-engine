#include "Launcher/ProjectLauncher.h"
#include "SavedProjects/ISavedProjects.h"
#include "Utilities/TimeUtilities.h"
#include "Core/Base.h"
#include <optional>

namespace Dwarf
{
  ProjectLauncher::ProjectLauncher(std::shared_ptr<IDwarfLogger>         logger,
                                   std::unique_ptr<IProjectLauncherView> view,
                                   std::shared_ptr<IProjectLauncherData> data,
                                   std::shared_ptr<ISavedProjects> projectList)
    : m_View(std::move(view))
    , m_Data(data)
    , m_Logger(logger)
    , m_SavedProjects(projectList)
  {
    m_Logger->LogDebug(Log("Creating ProjectLauncher", "ProjectLauncher"));
  }

  ProjectLauncher::~ProjectLauncher()
  {
    m_Logger->LogDebug(Log("Destroying ProjectLauncher", "ProjectLauncher"));
  }

  std::optional<SavedProject>
  ProjectLauncher::Run()
  {
    m_Logger->LogInfo(Log("Running project launcher...", "ProjectLauncher"));

    m_Logger->LogInfo(Log("Showing window...", "ProjectLauncher"));
    m_View->Show();

    m_Logger->LogInfo(Log("Starting main loop...", "ProjectLauncher"));
    while ((m_Data->GetState() != ProjectChooserState::Done) &&
           (m_Data->GetState() != ProjectChooserState::Cancelled))
    {
      TimeStamp currentTimeStamp = TimeUtilities::GetCurrent();

      m_View->Render();

      // TODO: Fps lock
      while (TimeUtilities::GetDifferenceInSeconds(
               TimeUtilities::GetCurrent(), currentTimeStamp) < (1.0 / 60.0))
      {
        // TODO: Update this when implementing multi threading
      }
    }

    m_Logger->LogInfo(Log("Project launcher finished", "ProjectLauncher"));
    if (m_Data->GetState() == ProjectChooserState::Done)
    {
      if (m_Data->GetSelectedProject())
      {
        m_SavedProjects->RegisterProjectOpening(
          m_Data->GetSelectedProject()->Path);
      }
      return m_Data->GetSelectedProject();
    }

    return std::nullopt;
  }
}