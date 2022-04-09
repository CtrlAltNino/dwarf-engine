#pragma once

#include "../../../utilities/dpch.h"

#include<imgui_internal.h>

#include "../IModule.h"
#include "../../windowing/IWindowManager.h"

class PerformanceModule : public IModule{
    private:
        double* frametime;
        double* renderTime;
    public:
        PerformanceModule(IViewListener *listener, double* frametime, double* renderTime, int id);
        void RenderModuleWindow() override;
        void StartFrame() override {}
};