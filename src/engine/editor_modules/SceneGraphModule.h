#include"IModule.h"
#include"../scene/Scene.h"

class SceneGraphModule : IModule {
    SceneGraphModule(Scene *scene);
    void RenderModuleWindow() override;
};