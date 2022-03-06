#pragma once

#include "IModule.h"
#include<iostream>

class PerformanceModule : public IModule{
    private:
        float* frametime;
    public:
        PerformanceModule(float* frametime);
        void RenderModuleWindow() override;
};