#include "IModule.h"
#include<iostream>

class PerformanceModule : public IModule{
    private:
        float* frametime;
    public:
        PerformanceModule(std::string moduleName, float* frametime);
        void RenderModuleWindow() override;
};