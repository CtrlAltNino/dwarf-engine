#pragma once

#include "../IModule.h"
#include<iostream>
#include<imgui_internal.h>

class PerformanceModule : public IModule{
    private:
        float* frametime;
    public:
        PerformanceModule(IViewListener *listener, float* frametime, int id);
        void RenderModuleWindow() override;
};