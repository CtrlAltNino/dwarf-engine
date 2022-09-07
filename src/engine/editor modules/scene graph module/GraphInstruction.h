#pragma once
#include "../../scene/Scene.h"

class GraphInstruction {
    protected:
        Scene* scene;
    public:
        GraphInstruction(Scene* scene):scene(scene){}
        virtual void PerformInstruction()=0;
};