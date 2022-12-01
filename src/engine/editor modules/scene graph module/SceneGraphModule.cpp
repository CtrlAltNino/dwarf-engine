#include "SceneGraphModule.h"

SceneGraphModule::SceneGraphModule(IViewListener *listener, Scene *scene, IInputManager* inputManager, int index)
:IModule(listener, "Scene graph", MODULE_TYPE::SCENE_GRAPH, index), inputManager(inputManager), scene(scene){ }

void SceneGraphModule::DrawNode(entt::entity entity){
    // Building the Entity
    Entity ent = Entity(entity, (EntityProvider*)scene);
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    std::string objectLabel = ent.GetComponent<TagComponent>().Tag;
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    // Splitting the channel to draw an extra rect when selected
    draw_list->ChannelsSplit(2);
    draw_list->ChannelsSetCurrent(1);

    // Bool do indicate if the node, which has children, is folded out
    bool opened = false;
    
    // Drawing the node, depending on if it has children or not
    if(ent.GetComponent<TransformComponent>().children.size() != 0){
        opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent.GetHandle(), flags, objectLabel.c_str());
    }else{
        //ImGui::Indent();
        ImGui::Selectable(("       " + objectLabel).c_str());
    }

    // If this node is being dragged
    if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_AcceptBeforeDelivery)){
        if(!scene->isEntitySelected(ent)){
            if(inputManager->GetKey(KEYCODE::LEFT_CONTROL)){
                scene->addEntityToSelection(ent);
            }else{
                scene->selectEntity(ent);
            }
        }
        // Set payload to carry the index of our item (could be anything)
        ImGui::SetDragDropPayload("SceneGraphEntity", &(scene->selectedEntities), sizeof(scene->selectedEntities[0]) * scene->selectedEntities.size());
        ImGui::Text(objectLabel.c_str());
        ImGui::EndDragDropSource();
    }

    // If something is dropped on this node
    ImGui::PushStyleColor(ImGuiCol_DragDropTarget, ImVec4(0,0,0,0));
    
    if(ImGui::BeginDragDropTarget()){
        float heightFrac = -1;
        const ImGuiPayload* payload = ImGui::GetDragDropPayload();
        if(payload->IsDataType("SceneGraphEntity")){
            heightFrac = (ImGui::GetMousePos().y - ImGui::GetItemRectMin().y) / (ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y);

            if(heightFrac < 0.33f){
                ImVec2 p_min = ImGui::GetItemRectMin();
                ImVec2 p_max = ImGui::GetItemRectMax();
                
                ImVec2 p1 = ImVec2(p_min.x, p_min.y - 5);
                ImVec2 p2 = ImVec2(p_min.x, p_min.y + 5);
                ImVec2 p3 = ImVec2(p_min.x + 10, p_min.y);

                ImVec2 rectMin = ImVec2(p_min.x, p_min.y - 1);
                ImVec2 rectMax = ImVec2(p_max.x, p_min.y + 1);
                ImGui::GetForegroundDrawList()->AddTriangleFilled(p1, p2, p3, IM_COL32(76, 86, 106, 255));
                ImGui::GetWindowDrawList()->AddRectFilled(rectMin, rectMax, IM_COL32(76, 86, 106, 255), 10);
            }else if(heightFrac > 0.66f){
                ImVec2 p_min = ImGui::GetItemRectMin();
                ImVec2 p_max = ImGui::GetItemRectMax();

                ImVec2 p1 = ImVec2(p_min.x, p_max.y - 5);
                ImVec2 p2 = ImVec2(p_min.x, p_max.y + 5);
                ImVec2 p3 = ImVec2(p_min.x + 10, p_max.y);

                ImVec2 rectMin = ImVec2(p_min.x, p_max.y - 1);
                ImVec2 rectMax = ImVec2(p_max.x, p_max.y + 1);
                ImGui::GetForegroundDrawList()->AddTriangleFilled(p1, p2, p3, IM_COL32(76, 86, 106, 255));
                ImGui::GetWindowDrawList()->AddRectFilled(rectMin, rectMax, IM_COL32(76, 86, 106, 255), 10);
            }else {
                draw_list->ChannelsSetCurrent(0);
                ImVec2 p_min = ImGui::GetItemRectMin();
                ImVec2 p_max = ImGui::GetItemRectMax();
                ImU32 rectCol = IM_COL32(76, 86, 106, 255);
                ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 10);
            }
        }

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SceneGraphEntity")) {
            IM_ASSERT(payload->DataSize == (sizeof(scene->selectedEntities[0]) * scene->selectedEntities.size()));

            std::vector<Entity> payload_e = *(const std::vector<Entity>*)payload->Data;
            std::vector<Entity>::iterator it;

            if(heightFrac < 0.33f){
                instructions.push_back(new NewParentInstruction(scene, payload_e, ent.GetComponent<TransformComponent>().parent));
                instructions.push_back(new ChildIndexInstruction(scene, payload_e, ent.GetChildIndex()));
            }else if(heightFrac > 0.66f){
                //droppedEntity.SetParent(ent.GetComponent<TransformComponent>().parent);
                //droppedEntity.SetChildIndex(ent.GetChildIndex()+1);
                instructions.push_back(new NewParentInstruction(scene, payload_e, ent.GetComponent<TransformComponent>().parent));
                instructions.push_back(new ChildIndexInstruction(scene, payload_e, ent.GetChildIndex()+1));
            }else {
                //droppedEntity.SetParent(entity);
                instructions.push_back(new NewParentInstruction(scene, payload_e, entity));
            }
        }
        ImGui::EndDragDropTarget();
    }

    if(ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left)){
        if(scene->isEntitySelected(ent)){
            // Set to selected object
            if(inputManager->GetKey(KEYCODE::LEFT_CONTROL)){
                scene->removeEntityFromSelection(ent);
            }else{
                scene->selectEntity(ent);
            }
        }else{
            // Set to selected object
            if(inputManager->GetKey(KEYCODE::LEFT_CONTROL)){
                scene->addEntityToSelection(ent);
            }else{
                scene->selectEntity(ent);
            }
        }
    }

    // Drawing an extra rect behind the node if its selected
    if(scene->isEntitySelected(ent)){
        draw_list->ChannelsSetCurrent(0);
        ImVec2 p_min = ImGui::GetItemRectMin();
        ImVec2 p_max = ImGui::GetItemRectMax();
        ImU32 rectCol = IM_COL32(26, 86, 186, 255);
        ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 10);
    }
    
    ImGui::PopStyleColor();

    // Merge channels
    draw_list->ChannelsMerge();
    // Draw the child nodes if the node has children and is folded out
    if(opened){
        for(entt::entity child : ent.GetComponent<TransformComponent>().children){
            DrawNode(child);
        }
        ImGui::TreePop();
    }
}

void SceneGraphModule::ProcessInstructions(){
    for(GraphInstruction* instruction : instructions){
        instruction->PerformInstruction();
    }

    instructions.clear();
}

void SceneGraphModule::RenderModuleWindow(){
    ImGuiWindowFlags window_flags = 0;

	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	//window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_MenuBar;

    if (!ImGui::Begin(GetIdentifier().c_str(), &windowOpened, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
    ImGui::PushStyleColor(ImGuiCol_DragDropTarget, ImVec4(0,0,0,0));

    auto view = scene->m_Registry.view<TransformComponent, TagComponent, IDComponent>();
    std::vector<entt::entity>* container = &(scene->rootEntity.GetComponent<TransformComponent>().children);

    int it = 0;
    while((container->begin() + it) != container->end()){
        DrawNode(*(container->begin() + it));
        it++;
    }

    ImGui::Dummy(ImGui::GetContentRegionAvail());

    if(ImGui::IsItemClicked()){
        scene->clearSelection();
    }

    // If something is dropped on this node
    if(ImGui::BeginDragDropTarget()){
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SceneGraphEntity")) {
            IM_ASSERT(payload->DataSize == (sizeof(scene->selectedEntities[0]) * scene->selectedEntities.size()));

            std::vector<Entity> payload_e = *(const std::vector<Entity>*)payload->Data;
            instructions.push_back(new NewParentInstruction(scene, payload_e, scene->rootEntity.GetHandle()));
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::PopStyleColor();
    ImGui::End();


    ProcessInstructions();

    if(!windowOpened)
        viewListener->RemoveWindow(moduleIndex);
}