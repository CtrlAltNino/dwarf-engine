#include "dpch.h"
#include "Launcher/ProjectLauncher.h"
#include "Launcher/ProjectListHandler.h"
#include "Utilities/TimeUtilities.h"

namespace Dwarf {

    ProjectLauncher* ProjectLauncher::s_Instance = nullptr;

    ProjectLauncher* CreateLauncher(){
        return new ProjectLauncher();
    }

    ProjectLauncher::ProjectLauncher() {
        s_Instance = this;
    }

    std::filesystem::path ProjectLauncher::Run() {
        // Initializing the project launcher model (e.g. loading the project list)
        WindowProps props("Dwarf Engine Project Launcher", 1100, 600);
        
        #if defined(_WIN32) || defined(__linux__)
            props.Api = GraphicsApi::OpenGL;
        #endif
        
        m_Window = Window::Create(props);

        m_Model = CreateScope<ProjectLauncherModel>(ProjectLauncherModel());
        m_Model->Init();

        // Initializing the view
        m_View = CreateScope<ProjectLauncherView>(ProjectLauncherView());
        m_View->Init(m_Model);
        
        m_Window->ShowWindow();

        while (((m_Model->GetState() != ProjectChooserState::Done) && (m_Model->GetState() != ProjectChooserState::Canceled)) && !m_Window->ShouldClose()) {
            TimeStamp currentFrameStamp = TimeUtilities::GetCurrent();

            m_Window->NewFrame();
            
            m_View->Render();

            m_Window->EndFrame();

            // TODO: Fps lock
            while(TimeUtilities::GetDifferenceInSeconds(TimeUtilities::GetCurrent(), currentFrameStamp) < (1.0 / 144.0)){
				// TODO: Update this when implementing multi threading
			}
        }

        int selectedProjectId = m_Model->GetSelectedProjectID();
        std::filesystem::path projectPath;

        if (m_Model->GetState() == ProjectChooserState::Done) {
            ProjectInformation projectInformation = ProjectListHandler::GetProjectInformation(selectedProjectId);
            if(projectInformation.name != ""){
                projectPath = projectInformation.path;
                ProjectListHandler::RegisterProjectOpening(selectedProjectId);
            }
        }
        
        return projectPath;
    }
}