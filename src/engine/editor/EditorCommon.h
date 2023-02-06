#pragma once

#include "../../utilities/dpch.h"

/// @brief Enum defining the types of GUI modules.
enum class MODULE_TYPE { PERFORMANCE, SCENE_GRAPH, CONSOLE, SCENE_VIEWER, ASSET_BROWSER, INSPECTOR, DEBUG };

class IModelListener{
    
};

/// @brief Interface to expose view related functionality.
class IViewListener{
    public:
        /// @brief Adds a new IMGUI window for a GUI module.
        /// @param moduleType Type of module to add a window for.
        virtual void AddWindow(MODULE_TYPE moduleType)=0;

        /// @brief Removes a IMGUI window that corresponds to a given index.
        /// @param index Index of the window to remove.
        virtual void RemoveWindow(int index)=0;

        /// @brief Returns the delta time of the last frame.
        /// @return The delta time in miliseconds. (TODO: CHECK)
        virtual float GetDeltaTime()=0;
};