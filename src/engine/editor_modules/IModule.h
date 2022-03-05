#include <imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>

#define MIN_HEIGHT 400
#define MIN_WIDTH 200

class IModule {
    public:
        virtual void RenderModuleWindow() {};
};