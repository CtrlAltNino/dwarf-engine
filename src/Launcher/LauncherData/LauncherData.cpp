#include "LauncherData.h"
#include <fmt/format.h>

namespace Dwarf
{
  LauncherData::LauncherData(std::shared_ptr<IDwarfLogger> logger)
    : mLogger(logger)
    , mState()
    , mSelectedProject()
  {
    mLogger->LogDebug(
      Log("ProjectLauncherData created.", "ProjectLauncherData"));
  }

  LauncherData::~LauncherData()
  {
    mLogger->LogDebug(
      Log("ProjectLauncherData destroyed.", "ProjectLauncherData"));
  }

  ProjectChooserState
  LauncherData::GetState() const
  {
    return mState;
  }

  void
  LauncherData::SetState(ProjectChooserState state)
  {
    std::string stateString = "";
    switch (state)
    {
      case ProjectChooserState::Choosing: stateString = "Choosing"; break;
      case ProjectChooserState::Done: stateString = "Done"; break;
      case ProjectChooserState::ProjectNotFound:
        stateString = "ProjectNotFound";
        break;
      case ProjectChooserState::CreateNewProject:
        stateString = "CreateNewProject";
        break;
      case ProjectChooserState::ChangeGraphicsApi:
        stateString = "ChangeGraphicsApi";
        break;
      case ProjectChooserState::Cancelled: stateString = "Cancelled"; break;
    }
    mLogger->LogDebug(
      Log(fmt::format("Setting project launcher state to: {}", stateString),
          "ProjectLauncherData"));
    mState = state;
  }

  void
  LauncherData::SetSelectedProject(std::optional<SavedProject> project)
  {
    mSelectedProject = project;
  }

  const std::optional<SavedProject>&
  LauncherData::GetSelectedProject() const
  {
    return mSelectedProject;
  }
} // namespace Dwarf