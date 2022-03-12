#pragma once

#include"EditorCommon.h"
#include"../scene/Scene.h"

class EditorModel{
    private:
        Scene scene;
        IModelListener* modelListener;
    public:
        EditorModel(IModelListener* listener);
        void InitScene(std::string projectPath);
        Scene* GetScene();
};