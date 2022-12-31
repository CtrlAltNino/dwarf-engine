#pragma once

#include "../../utilities/dpch.h"

#include"../../utilities/Common.h"
#include"../../utilities/TimeUtilities.h"
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
#include"../editor modules/inspector module/InspectorModule.h"
#include"../asset database/AssetDatabase.h"

/// @brief The controller part of the editors MVC structure.
class EditorController : public IModelListener, public IViewListener {
    private:
        /// @brief Pointer to the window manager instance of this editor instance.
        IWindowManager* windowManager = nullptr;

        /// @brief Pointer to the input manager instance of this editor instance.
        IInputManager* inputManager = nullptr;

        /// @brief The MVC model instance of this editor instance.
        EditorModel editorModel;

        /// @brief The MVC view instance of this editor instance.
        EditorView editorView;

        /// @brief Database managing the assets of a project.
        AssetDatabase assetDatabase;

        /// @brief ID counter for GUI modules.
        int guiModuleIDCount = 0;

        /// @brief List of GUI modules.
        std::vector<IModule*> guiModules;

        /// @brief Path to the opened project.
        std::string projectPath;

        /// @brief Current delta time
        double deltaTime;

        /// @brief Time it took for the rendering
        double renderTime;
        //void InitScene(std::string projectPath);
    public:
        EditorController(ProjectData projectData);

        /// @brief Starts the render loop
        void RunLoop();
        //virtual void UpdateEditorCamera(Camera* camera);

        /// @brief Adds a new IMGUI window for a GUI module.
        /// @param moduleType Type of module to add a window for.
        virtual void AddWindow(MODULE_TYPE moduleType);

        /// @brief Removes a IMGUI window that corresponds to a given index.
        /// @param index Index of the window to remove.
        virtual void RemoveWindow(int index);

        /// @brief Returns the delta time of the last frame.
        /// @return The delta time in miliseconds. (TODO: CHECK)
        virtual float GetDeltaTime();
};