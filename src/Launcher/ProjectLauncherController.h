#pragma once

#include "Launcher/ProjectLauncherModel.h"
#include "Launcher/ProjectLauncherView.h"
#include "Window/IWindowManager.h"
#include "Input/IInputManager.h"

#include <SDL2/SDL.h>
#if defined(_WIN32) || defined(__linux__)
    #include <SDL2/SDL_opengl.h>
    #include "Window/WindowManagerOpenGL.h"
    #include "Input/InputManagerOpenGL.h"
#elif __APPLE__
    #include <SDL2/SDL_metal.h>
    #include "Input/InputManagerOpenGL.h"
    #include "Window/WindowManagerMetal.h"
#endif

namespace Dwarf {

    class ProjectLauncherController {
        private:
            ProjectLauncherModel model;
            ProjectLauncherView view;
            IWindowManager* windowManager = nullptr;
            IInputManager* inputManager = nullptr;
        public:
            ProjectLauncherController();
            void Init();
            void Quit();
            std::filesystem::path Run();
    };
}