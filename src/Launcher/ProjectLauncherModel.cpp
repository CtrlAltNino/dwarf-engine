#include "dpch.h"

#include "Launcher/ProjectLauncherModel.h"

#include "Launcher/ProjectCreator.h"
#include "Launcher/ProjectListHandler.h"
#include "Utilities/FileHandler.h"

namespace Dwarf {

  ProjectLauncherModel::ProjectLauncherModel() {}

  void ProjectLauncherModel::Init()
  {
    ProjectListHandler::LoadProjectList();
    ProjectCreator::InitProjectCreator();
  }

  ProjectChooserState ProjectLauncherModel::GetState()
  {
    return this->m_State;
  }

  void ProjectLauncherModel::SetState(ProjectChooserState state)
  {
    this->m_State = state;
  }

  int ProjectLauncherModel::GetSelectedProjectID()
  {
    return this->m_SelectedProjectId;
  }

  void ProjectLauncherModel::SetSelectedProjectID(int id)
  {
    this->m_SelectedProjectId = id;
  }
}