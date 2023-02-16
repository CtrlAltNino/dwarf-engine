#include "Launcher/ProjectLauncherController.h"

namespace Dwarf {

    #define LAUNCHER_INITIAL_WINDOW_WIDTH (960)
    #define LAUNCHER_INITIAL_WINDOW_HEIGHT (540)

    ProjectLauncherController::ProjectLauncherController() { }

    void ProjectLauncherController::Init(){
        // Initializing SDL2
        SDL_SetMainReady();
        SDL_InitSubSystem(SDL_INIT_VIDEO);
        
        #if defined(_WIN32) || defined(__linux__)
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        #elif __APPLE__
            SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
        #endif

        // Initializing the window manager and creating the window
        #if defined(_WIN32) || defined(__linux__)
            inputManager = new InputManagerOpenGL();
            windowManager = new WindowManagerOpenGL((InputManagerOpenGL*)inputManager);
        #elif __APPLE__
            inputManager = new InputManagerOpenGL();
            windowManager = new WindowManagerMetal((InputManagerOpenGL*)inputManager);
        #endif

        windowManager->Init();
        windowManager->InitWindow({LAUNCHER_INITIAL_WINDOW_WIDTH, LAUNCHER_INITIAL_WINDOW_HEIGHT},
            {LAUNCHER_INITIAL_WINDOW_WIDTH, LAUNCHER_INITIAL_WINDOW_HEIGHT},
            "Dwarf Engine Project Launcher");
        windowManager->ShowWindow();
        
        // Initializing the project launcher model (e.g. loading the project list)
        model.Init();

        // Initializing the view
        view.Init(windowManager, &model);
    }

    void ProjectLauncherController::Quit() {
        
    }

    std::filesystem::path ProjectLauncherController::Run() {
        windowManager->ShowWindow();
        
        SDL_Event Event;

        while (((model.GetState() != ProjectChooserState::Done) && (model.GetState() != ProjectChooserState::Canceled))) {
            while(SDL_PollEvent(&Event)) {
                ImGui_ImplSDL2_ProcessEvent(&Event);
                if (Event.type == SDL_QUIT)
                {
                    model.SetState(ProjectChooserState::Canceled);
                }
            }
            
            
            //SDL_GetWindowSize(window, &windowInfo.width, &windowInfo.height);
            //glViewport(0, 0, windowInfo.width, windowInfo.height);
            //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            windowManager->StartFrame();
            
            view.Render();

            windowManager->EndFrame();

            // TODO: Fps lock
            
            //SDL_GL_SwapWindow(window);
        }

        //ProjectData projectData = ProjectData();

        int selectedProjectId = model.GetSelectedProjectID();
        std::filesystem::path projectPath;

        if (model.GetState() == ProjectChooserState::Done) {
            ProjectInformation projectInformation = ProjectListHandler::GetProjectInformation(selectedProjectId);
            if(projectInformation.name != ""){
                //projectData.name = projectInformation.name;
                //projectData.path = projectInformation.path;
                //projectData.graphicsApi = (GraphicsApi)projectInformation.graphicsApi;
                projectPath = projectInformation.path;
                ProjectListHandler::RegisterProjectOpening(selectedProjectId);

                //Editor editor = Editor(projectData);
                //editor.Run();
                //EditorController editor(projectData);
                //editor.RunLoop();
            }
        }

        windowManager->Quit();
        Quit();
        
        return projectPath;
        //return projectData;
    }
}