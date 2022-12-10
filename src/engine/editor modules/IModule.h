#pragma once

#include "../../utilities/dpch.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_internal.h>

#include"../editor/EditorCommon.h"

/// @brief GUI module base class.
class IModule {
    protected:
        /// @brief Name to use for the module.
        std::string moduleLabel;

        /// @brief Enum identification.
        MODULE_TYPE moduleType;

        /// @brief Incremented global GUI module ID.
        int moduleIndex;

        /// @brief Interfaced editor controller to communicate with the controller.
        IViewListener *viewListener;

        /// @brief Flag to check if the window is collapsed or not.
        bool windowOpened = true;
    public:
        IModule(IViewListener *listener, std::string name, MODULE_TYPE type, int index)
            : viewListener(listener),
            moduleLabel(name),
            moduleType(type),
            moduleIndex(index)
            {}
        
        /// @brief Returns the name of the module.
        /// @return Name of the module.
        std::string GetModuleName() {return moduleLabel;}

        /// @brief Returns thhe type of the module.
        /// @return Type of the module.
        MODULE_TYPE GetModuleType() {return moduleType;}

        /// @brief Returns the global module index.
        /// @return The module index.
        int GetIndex() {return moduleIndex;}

        /// @brief Generates the ImGui window identifier for a module window..
        /// @return The window identifier.
        std::string GetIdentifier(){
            return (moduleLabel + "##" + std::to_string(moduleIndex));
        }

        /// @brief Generates a ImGui ID for the module.
        /// @return The ImGui ID.
        ImGuiID GetImGuiID(){
            return ImGui::GetID(GetIdentifier().c_str());
        }

        /// @brief Renders the module window.
        virtual void RenderModuleWindow()=0;

        /// @brief Executes all pre frame tasks.
        virtual void StartFrame()=0;
};