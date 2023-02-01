#pragma once

#include "ProjectLauncherModel.h"
#include "ProjectLauncherView.h"
#include "../windowing/IWindowManager.h"
#include "../input/IInputManager.h"
#include<SDL2/SDL.h>
#if defined(_WIN32) || defined(__linux__)
    #include "../windowing/WindowManagerOpenGL.h"
    #include "../input/InputManagerOpenGL.h"
    #include<SDL2/SDL_opengl.h>
#elif __APPLE__
    #include<SDL2/SDL_metal.h>
    #include "../input/InputManagerOpenGL.h"
    #include "../windowing/WindowManagerMetal.h"
#endif

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
        ProjectData Run();
};