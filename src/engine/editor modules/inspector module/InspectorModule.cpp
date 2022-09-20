#include "InspectorModule.h"

InspectorModule::InspectorModule(IViewListener *listener, Scene* scene, int id)
    :IModule(listener, "Inspector", MODULE_TYPE::INSPECTOR, id), scene(scene){
		/*std::cout << "Module created with:" << std::endl;
		std::cout << "Name: " << moduleLabel << std::endl;
		std::cout << "Index: " << moduleIndex << std::endl;
		std::cout << "Identifier: " << GetIdentifier() << std::endl;*/
}
void InspectorModule::RenderModuleWindow(){
    ImGuiWindowFlags window_flags = 0;

	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;
	//window_flags |= ImGuiWindowFlags_NoCollapse;
	//window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_MenuBar;

	//static bool b_open = true;

    //if (!ImGui::Begin((moduleLabel + "##" + std::to_string(index)).c_str(), NULL, window_flags))
    if (!ImGui::Begin(GetIdentifier().c_str(), &windowOpened, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
        ImGui::PopStyleVar();
		return;
	}

    if(scene->selectedEntities.size() == 1) {
        RenderComponents(scene->selectedEntities[0]);
    }

    ImGui::End();


	
    if(!windowOpened)
        viewListener->RemoveWindow(moduleIndex);
}



void InspectorModule::RenderComponents(Entity entity) {
    if(entity.HasComponent<IDComponent>()){
        ImGui::Text(std::to_string(entity.GetComponent<IDComponent>().ID).c_str());
        
        ImGui::Separator();
    }
    

    if(entity.HasComponent<TagComponent>()){
        TagComponent* tc = &entity.GetComponent<TagComponent>();
        char* str0 = {(char*)tc->Tag.c_str()};
        ImGui::InputText("Name", str0, sizeof(char) * 64);
        tc->Tag = std::string(str0);
    
        ImGui::Separator();
    }

    if(entity.HasComponent<TransformComponent>()){
        // Render Position
        // Render Rotation
        // Render Scale

        ImGui::Text("Transform Component");
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);

        TransformComponent* transform = &entity.GetComponent<TransformComponent>();

        ImGui::DragFloat3("Position", &transform->position.x, 0.015f);
        static const float rad_2_deg = 180.0f / std::_Pi;
        static const float deg_2_rad = std::_Pi / 180.0f;
        //glm::vec3 rot = rad_2_deg * glm::eulerAngles(transform->rotation);
        glm::vec3 rot = transform->getEulerAngles();
        ImGui::DragFloat3("Rotation", &rot.x, 0.05f);
        transform->rotation = rot;
        ImGui::DragFloat3("Scale", &transform->scale.x, 0.015f);
        
        ImGui::Separator();
    }

    if(entity.HasComponent<LightComponent>()){
        // Render dropdown for for light type
        // Render type depending variables
        ImGui::Text("Light Component");
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
        
        LightComponent* lc = &entity.GetComponent<LightComponent>();

        static int item_current = 0;
        ImGui::Combo("Light type", &item_current, lightTypeNames, IM_ARRAYSIZE(lightTypeNames));
        lc->type = (LIGHT_TYPE)item_current;

        ImGui::ColorEdit3("Color", (float*)&lc->lightColor.r, ImGuiColorEditFlags_None);
        ImGui::DragFloat("Attenuation", &lc->attenuation, 0.015f);
        
        if(lc->type == POINT_LIGHT){
            ImGui::DragFloat("Radius", &lc->radius, 0.015f);
        }
        
        if(lc->type == SPOT_LIGHT){
            ImGui::SliderFloat("Opening Angle", &lc->openingAngle, 0.0f, 180.0f);
        }
        
        ImGui::Separator();
    }

    if(entity.HasComponent<MeshRendererComponent>()){
        ImGui::Text("Mesh Renderer Component");
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
        
        // Render Mesh asset reference
        // Render Materials
        // Render check box for casting shadows
        MeshRendererComponent* mrc = &entity.GetComponent<MeshRendererComponent>();
        
        // TODO: Slot for a mesh asset
        char* meshAssetName = (char*)"Yeet";
        ImGui::InputText("Mesh", meshAssetName, sizeof(meshAssetName), ImGuiInputTextFlags_ReadOnly);
        //ImGuiInputTextFlags_ReadOnly

        ImGui::Text("Materials");
        
        // TODO: Slots for material assets
        for(int i = 0; i < mrc->materials.size(); i++) {
            Material currentMat = mrc->materials[i];
            ImGui::Indent(10.0f);
            ImGui::Text((std::to_string(i) + std::string(" - ") + std::string(currentMat.name)).c_str());
        }

        ImGui::Separator();
    }
}