#pragma once

#include "../../utilities/dpch.h"

#include"../../utilities/Common.h"
#include"EditorCommon.h"
#include"EditorModel.h"
#include"EditorView.h"
#include"../input/IInputManager.h"
#include"../windowing/IWindowManager.h"
#include"../windowing/WindowManagerOpenGL.h"
#include"../input/InputManagerOpenGL.h"
#include"EditorProperties.h"
#include"../editor modules/IModule.h"
#include"../editor modules/performance module/PerformanceModule.h"
#include"../editor modules/scene graph module/SceneGraphModule.h"
#include"../editor modules/scene viewer module/SceneViewerModule.h"
#include"../editor modules/asset browser module/AssetBrowserModule.h"

class EditorController : public IModelListener, public IViewListener {
    private:
        IWindowManager* windowManager = nullptr;
        IInputManager* inputManager = nullptr;
        EditorModel editorModel;
        EditorView editorView;
        int guiModuleIDCount = 0;
        std::vector<IModule*> guiModules;
        std::string projectPath;
        double deltaTime;
        double renderTime;
        //void InitScene(std::string projectPath);
    public:
        EditorController(ProjectData projectData);
        void RunLoop();
        //virtual void UpdateEditorCamera(Camera* camera);
        virtual void AddWindow(MODULE_TYPE moduleType);
        virtual void RemoveWindow(int index);
        virtual float GetDeltaTime();
};