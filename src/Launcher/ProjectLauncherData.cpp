#include "ProjectLauncherData.h"

namespace Dwarf
{
  ProjectLauncherData::ProjectLauncherData(std::shared_ptr<IDwarfLogger> logger)
    : m_Logger(logger)
    , m_State()
    , m_SelectedProject()
  {
    m_Logger->LogInfo(
      Log("ProjectLauncherData created.", "ProjectLauncherData"));
  }

  ProjectLauncherData::~ProjectLauncherData()
  {
    m_Logger->LogInfo(
      Log("ProjectLauncherData destroyed.", "ProjectLauncherData"));
  }

  ProjectChooserState
  ProjectLauncherData::GetState() const
  {
    return m_State;
  }

  void
  ProjectLauncherData::SetState(ProjectChooserState state)
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
    m_Logger->LogInfo(
      Log(fmt::format("Setting project launcher state to: {}", stateString),
          "ProjectLauncherData"));
    m_State = state;
  }

  void
  ProjectLauncherData::SetSelectedProject(const ProjectInformation& project)
  {
    m_SelectedProject = project;
  }

  const ProjectInformation&
  ProjectLauncherData::GetSelectedProject() const
  {
    return m_SelectedProject;
  }
} // namespace Dwarf