#pragma once

#include <imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_internal.h>
#include<string>

#include"../editor/EditorCommon.h"

class IModule {
    protected:
        std::string moduleLabel;
        MODULE_TYPE moduleType;
        int moduleIndex;
        IViewListener *viewListener;
        bool windowOpened = true;
    public:
        IModule(IViewListener *listener, std::string name, MODULE_TYPE type, int index)
            : viewListener(listener),
            moduleLabel(name),
            moduleType(type),
            moduleIndex(index)
            {}
        std::string GetModuleName() {return moduleLabel;}
        MODULE_TYPE GetModuleType() {return moduleType;}
        int GetIndex() {return moduleIndex;}
        std::string GetIdentifier(){
            return (moduleLabel + "##" + std::to_string(moduleIndex));
        }
        ImGuiID GetImGuiID(){
            return ImGui::GetID(GetIdentifier().c_str());
        }
        virtual void RenderModuleWindow()=0;
        virtual void StartFrame()=0;
};