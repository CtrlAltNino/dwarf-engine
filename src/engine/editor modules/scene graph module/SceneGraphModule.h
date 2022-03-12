#include"../IModule.h"
#include"../../scene/Scene.h"
#include"../../editor/EditorCommon.h"
#include<string>

class SceneGraphModule : public IModule {
    private:
        Scene *scene;
    public:
        SceneGraphModule(IViewListener *listener, Scene *scene, int index);
        void RenderModuleWindow() override;
        void StartFrame() override {}
};