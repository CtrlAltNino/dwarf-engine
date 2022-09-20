#pragma once

#include "../../../utilities/dpch.h"
#include"../../scene/Scene.h"

#include<imgui_internal.h>
#include "../IModule.h"

class InspectorModule : public IModule{
    private:
        Scene* scene;
        void RenderComponents(Entity entity);
    public:
        InspectorModule(IViewListener *listener, Scene* scene, int id);
        void RenderModuleWindow() override;
        void StartFrame() override {}
};