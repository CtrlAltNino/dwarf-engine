#pragma once

#include "Utilities/Common.h"

#include "Input/IInputManager.h"

#include "Window/IWindowManager.h"
#include "Window/WindowManagerOpenGL.h"
#ifdef __APPLE__
#include "Window/WindowManagerMetal.h"
#endif

#include "Core/Asset/AssetDatabase.h"
#include "Editor/EditorCommon.h"
#include "Editor/EditorModel.h"
#include "Editor/EditorView.h"
#include "Editor/EditorProperties.h"
#include "Editor/Modules/IModule.h"
#include "Editor/Modules/Performance Viewer/PerformanceModule.h"
#include "Editor/Modules/Scene Hierarchy/SceneGraphModule.h"
#include "Editor/Modules/Scene Viewer/SceneViewerModule.h"
#include "Editor/Modules/Asset Browser/AssetBrowserModule.h"
#include "Editor/Modules/Inspector/InspectorModule.h"
#include "Editor/Modules/Debug Console/DebugModule.h"
#include "Input/InputManagerOpenGL.h"
#include "Utilities/TimeUtilities.h"

namespace Dwarf {

    /// @brief The controller part of the editors MVC structure.
    class EditorController : public IModelListener, public IViewListener
    {
    private:
        /// @brief Pointer to the window manager instance of this editor instance.
        IWindowManager *windowManager = nullptr;

        /// @brief Pointer to the input manager instance of this editor instance.
        IInputManager *inputManager = nullptr;

        /// @brief The MVC model instance of this editor instance.
        EditorModel editorModel;

        /// @brief The MVC view instance of this editor instance.
        EditorView editorView;

        /// @brief Database managing the assets of a project.
        AssetDatabase assetDatabase;

        /// @brief ID counter for GUI modules.
        int guiModuleIDCount = 0;

        /// @brief List of GUI modules.
        std::vector<IModule *> guiModules;

        /// @brief Path to the opened project.
        std::filesystem::path projectPath;

        std::string windowTitle;

        /// @brief Current delta time
        double deltaTime;

        /// @brief Time it took for the rendering
        double renderTime;
        // void InitScene(std::string projectPath);
    public:
        EditorController(std::filesystem::path projectPath);

        /// @brief Starts the render loop
        void RunLoop();
        // virtual void UpdateEditorCamera(Camera* camera);

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
}