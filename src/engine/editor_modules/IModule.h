#pragma once

#include <imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
#include<string>

#define MIN_HEIGHT 400
#define MIN_WIDTH 200

class IModule {
    protected:
        std::string moduleName;
    public:
        IModule(std::string moduleName):moduleName(moduleName){}
        virtual void RenderModuleWindow() {};
};