#include "pch.h"

#include "Launcher/ProjectLauncher.h"
#include "SavedProjects/ISavedProjects.h"
#include "Utilities/TimeUtilities.h"

namespace Dwarf
{
  ProjectLauncher::ProjectLauncher(std::shared_ptr<IDwarfLogger>         logger,
                                   std::unique_ptr<IProjectLauncherView> view,
                                   std::shared_ptr<ILauncherData>        data,
                                   std::shared_ptr<ISavedProjects> projectList)
    : mView(std::move(view))
    , mData(std::move(data))
    , mLogger(std::move(logger))
    , mSavedProjects(std::move(projectList))
  {
    mLogger->LogDebug(Log("Creating ProjectLauncher", "ProjectLauncher"));
  }

  ProjectLauncher::~ProjectLauncher()
  {
    mLogger->LogDebug(Log("Destroying ProjectLauncher", "ProjectLauncher"));
  }

  auto
  ProjectLauncher::Run() -> std::optional<SavedProject>
  {
    mLogger->LogInfo(Log("Running project launcher...", "ProjectLauncher"));

    mLogger->LogInfo(Log("Showing window...", "ProjectLauncher"));
    mView->Show();

    mLogger->LogInfo(Log("Starting main loop...", "ProjectLauncher"));
    while ((mData->GetState() != ProjectChooserState::Done) &&
           (mData->GetState() != ProjectChooserState::Cancelled))
    {
      TimeStamp currentTimeStamp = TimeUtilities::GetCurrent();

      mView->Render();

      // TODO: Fps lock
      while (TimeUtilities::GetDifferenceInSeconds(
               TimeUtilities::GetCurrent(), currentTimeStamp) < (1.0 / 60.0))
      {
        // TODO: Update this when implementing multi threading
      }
    }

    mLogger->LogInfo(Log("Project launcher finished", "ProjectLauncher"));
    if (mData->GetState() == ProjectChooserState::Done)
    {
      if (mData->GetSelectedProject())
      {
        mSavedProjects->RegisterProjectOpening(
          mData->GetSelectedProject()->Path);
      }
      return mData->GetSelectedProject();
    }

    return std::nullopt;
  }
}