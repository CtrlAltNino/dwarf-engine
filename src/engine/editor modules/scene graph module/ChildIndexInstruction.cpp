#include "ChildIndexInstruction.h"

ChildIndexInstruction::ChildIndexInstruction(Scene* scene, std::vector<Entity> sourceEntities, int index)
: GraphInstruction(scene), sourceEntities(sourceEntities), index(index){ }

void ChildIndexInstruction::PerformInstruction(){
    int startIndex = index;
    for(Entity ent : sourceEntities){
        ent.SetChildIndex(startIndex++);
    }
}