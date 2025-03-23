#include "LauncherData.h"
#include <fmt/format.h>

#include <utility>

namespace Dwarf
{
  LauncherData::LauncherData(std::shared_ptr<IDwarfLogger> logger)
    : mLogger(std::move(logger))
    , mState()
  {
    mLogger->LogDebug(
      Log("ProjectLauncherData created.", "ProjectLauncherData"));
  }

  LauncherData::~LauncherData()
  {
    mLogger->LogDebug(
      Log("ProjectLauncherData destroyed.", "ProjectLauncherData"));
  }

  auto
  LauncherData::GetState() const -> ProjectChooserState
  {
    return mState;
  }

  void
  LauncherData::SetState(ProjectChooserState state)
  {
    std::string stateString;
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

  auto
  LauncherData::GetSelectedProject() const -> const std::optional<SavedProject>&
  {
    return mSelectedProject;
  }
} // namespace Dwarf