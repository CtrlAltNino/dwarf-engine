#pragma once

#include "../../utilities/dpch.h"

#include<imgui_internal.h>

#include"../editor modules/IModule.h"
#include"EditorCommon.h"


class EditorView {
    private:
        ImGuiIO* io;
        IViewListener* viewListener;
        void RenderDockSpace();
    public:
        EditorView(IViewListener* listener);
        void Init();
        void StartFrame();
        void RenderGui();
        void EndFrame();
        void DockWindowToFocused(std::string windowName);
};