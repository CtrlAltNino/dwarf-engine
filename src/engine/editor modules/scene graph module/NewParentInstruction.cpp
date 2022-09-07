#include "NewParentInstruction.h"

NewParentInstruction::NewParentInstruction(Scene* scene, std::vector<Entity> sourceEntities, entt::entity newParent)
: GraphInstruction(scene), sourceEntities(sourceEntities), newParent(newParent){ }

void NewParentInstruction::PerformInstruction(){
    Entity p = Entity(newParent, scene);
    for(Entity ent : sourceEntities){
        if((p.GetComponent<TransformComponent>().parent != ent.GetHandle())
            && (ent.GetHandle() != newParent)){
                ent.SetParent(newParent);
        }
    }
}