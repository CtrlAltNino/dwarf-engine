#pragma once

// system
#include "../../utilities/dpch.h"
#include "ProjectLauncherUtilities.h"
#include "ProjectCreator.h"
#include "ProjectListHandler.h"
#include "../../utilities/FileHandler.h"

class ProjectLauncherModel {
    private:
        /// @brief Current state of the project launcher
        ProjectChooserState state = ProjectChooserState::Choosing;
        int selectedProjectId = -1;
    public:
        ProjectLauncherModel();
        void Init();
        ProjectChooserState GetState();
        void SetState(ProjectChooserState state);
        int GetSelectedProjectID();
        void SetSelectedProjectID(int id);
};