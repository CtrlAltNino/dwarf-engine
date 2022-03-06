#pragma once

#include<iostream>
#include "../project launcher/ProjectLauncherUtilities.h"
#include "../windowing/WindowManager.h"
#include"EditorGui.h"
#include"../scene/Scene.h"
#include"EditorProperties.h"
#include"InputHandler.h"
#include"../editor_modules/IModule.h"
#include"../editor_modules/PerformanceModule.h"

class Editor{
    private:
        Scene scene;
        void InitScene(std::string projectPath);
        void UpdateEditorCamera();
    public:
        Editor(ProjectData projectData);
        PerformanceModule* CreatePerformanceModule();
        Scene* GetScene();
        void Run();
};