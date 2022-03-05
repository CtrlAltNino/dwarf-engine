#include"IModule.h"
#include"../scene/Scene.h"
#include<string>

class SceneGraphModule : IModule {
    private:
        Scene *scene;

    public:
        SceneGraphModule(std::string moduleName, Scene *scene);
        void RenderModuleWindow() override;
};