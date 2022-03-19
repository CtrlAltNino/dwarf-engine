#pragma once

#include "../IModule.h"
#include "../../windowing/IWindowManager.h"
#include<string>
#include<iostream>
#include<imgui_internal.h>

class PerformanceModule : public IModule{
    private:
        double* frametime;
        double* renderTime;
    public:
        PerformanceModule(IViewListener *listener, double* frametime, double* renderTime, int id);
        void RenderModuleWindow() override;
        void StartFrame() override {}
};