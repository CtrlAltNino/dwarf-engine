#include "ProjectLauncherData.h"

namespace Dwarf
{
  ProjectLauncherData::ProjectLauncherData()
    : m_State()
    , m_SelectedProject()
  {
  }

  ProjectChooserState
  ProjectLauncherData::GetState() const
  {
    return m_State;
  }

  void
  ProjectLauncherData::SetState(ProjectChooserState state)
  {
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