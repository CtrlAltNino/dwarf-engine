#pragma once

#include"EditorCommon.h"
#include"../data structures/Entity.h"
#include"../scene/Scene.h"
#include"../scene/MeshLoader.h"

class EditorModel{
    private:
        Scene scene;
        IModelListener* modelListener;
    public:
        EditorModel(IModelListener* listener);
        void InitScene(std::string projectPath);
        void InitSceneNew(std::string projectPath);
        Scene* GetScene();
};