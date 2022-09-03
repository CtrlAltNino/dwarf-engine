#pragma once

#include "../../../utilities/dpch.h"


#include"../IModule.h"
#include"../../scene/Scene.h"
#include"../../editor/EditorCommon.h"
#include"../../scene/EntityProvider.h"
#include"../../input/IInputManager.h"

class SceneGraphModule : public IModule {
    private:
        Scene *scene;
        IInputManager* inputManager;
        void DrawNode(entt::entity entity);
    public:
        SceneGraphModule(IViewListener *listener, Scene *scene, IInputManager* inputManager, int index);
        void RenderModuleWindow() override;
        void StartFrame() override {}
};