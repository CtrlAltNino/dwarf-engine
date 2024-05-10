#include "pch.h"

#include "Launcher/ProjectLauncherModel.h"
#include "Launcher/ProjectCreator.h"
#include "Launcher/ProjectListHandler.h"

namespace Dwarf
{

  ProjectLauncherModel::ProjectLauncherModel()
  {
    ProjectListHandler::LoadProjectList();
    ProjectCreator::InitProjectCreator();
  }

  ProjectChooserState
  ProjectLauncherModel::GetState() const
  {
    return m_State;
  }

  void
  ProjectLauncherModel::SetState(ProjectChooserState state)
  {
    m_State = state;
  }

  int
  ProjectLauncherModel::GetSelectedProjectID() const
  {
    return m_SelectedProjectId;
  }

  void
  ProjectLauncherModel::SetSelectedProjectID(int id)
  {
    m_SelectedProjectId = id;
  }
}