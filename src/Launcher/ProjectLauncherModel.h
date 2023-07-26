#pragma once

#include "Core/Base.h"
#include "Launcher/ProjectLauncherUtilities.h"

namespace Dwarf {

    class ProjectLauncherModel {
        private:

            /// @brief Current state of the project launcher
            ProjectChooserState m_State = ProjectChooserState::Choosing;
            int m_SelectedProjectId = -1;
        public:

            ProjectLauncherModel();

            void Init();

            ProjectChooserState GetState();

            void SetState(ProjectChooserState state);

            int GetSelectedProjectID();

            void SetSelectedProjectID(int id);
    };
}