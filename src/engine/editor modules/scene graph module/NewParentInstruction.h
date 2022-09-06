#pragma once

#include "GraphInstruction.h"
#include "../../data structures/Entity.h"

class NewParentInstruction : public GraphInstruction {
    private:
        std::vector<Entity> sourceEntities;
        entt::entity newParent;
    public:
        virtual void PerformInstruction();
        NewParentInstruction(Scene* scene, std::vector<Entity> sourceEntities, entt::entity newParent);
};