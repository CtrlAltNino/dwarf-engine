#pragma once

#include "GraphInstruction.h"

class ChildIndexInstruction : public GraphInstruction {
    private:
        std::vector<Entity> sourceEntities;
        int index;
    public:
        virtual void PerformInstruction();
        ChildIndexInstruction(Scene* scene, std::vector<Entity> sourceEntities, int index);
};