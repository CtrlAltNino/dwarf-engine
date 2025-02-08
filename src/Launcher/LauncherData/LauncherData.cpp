#include "LauncherData.h"

namespace Dwarf
{
  LauncherData::LauncherData(std::shared_ptr<IDwarfLogger> logger)
    : m_Logger(logger)
    , m_State()
    , m_SelectedProject()
  {
    m_Logger->LogDebug(
      Log("ProjectLauncherData created.", "ProjectLauncherData"));
  }

  LauncherData::~LauncherData()
  {
    m_Logger->LogDebug(
      Log("ProjectLauncherData destroyed.", "ProjectLauncherData"));
  }

  ProjectChooserState
  LauncherData::GetState() const
  {
    return m_State;
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
    m_Logger->LogDebug(
      Log(fmt::format("Setting project launcher state to: {}", stateString),
          "ProjectLauncherData"));
    m_State = state;
  }

  void
  LauncherData::SetSelectedProject(std::optional<SavedProject> project)
  {
    m_SelectedProject = project;
  }

  const std::optional<SavedProject>&
  LauncherData::GetSelectedProject() const
  {
    return m_SelectedProject;
  }
} // namespace Dwarf