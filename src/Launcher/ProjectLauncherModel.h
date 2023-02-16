#pragma once

#include "Launcher/ProjectLauncherUtilities.h"
#include "Launcher/ProjectCreator.h"
#include "Launcher/ProjectListHandler.h"
#include "Utilities/FileHandler.h"

namespace Dwarf {

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
}