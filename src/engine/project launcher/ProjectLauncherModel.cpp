#include "ProjectLauncherModel.h"

ProjectLauncherModel::ProjectLauncherModel() {}

void ProjectLauncherModel::Init(){
    FileHandler::InitFileHandler();
	ProjectListHandler::LoadProjectList();
	ProjectCreator::InitProjectCreator();
}

ProjectChooserState ProjectLauncherModel::GetState() {
	return this->state;
}

void ProjectLauncherModel::SetState(ProjectChooserState state){
	this->state = state;
}

int ProjectLauncherModel::GetSelectedProjectID() {
	return this->selectedProjectId;
}

void ProjectLauncherModel::SetSelectedProjectID(int id) {
	this->selectedProjectId = id;
}