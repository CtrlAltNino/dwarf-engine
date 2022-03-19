#include "SceneGraphModule.h"

SceneGraphModule::SceneGraphModule(IViewListener *listener, Scene *scene, int index)
:IModule(listener, "Scene graph", MODULE_TYPE::SCENE_GRAPH, index), scene(scene){ }

void SceneGraphModule::RenderModuleWindow(){
    ImGuiWindowFlags window_flags = 0;

	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	//window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_MenuBar;

    //if (!ImGui::Begin((moduleLabel + "##" + std::to_string(index)).c_str(), NULL, window_flags))
    if (!ImGui::Begin(GetIdentifier().c_str(), &windowOpened, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

    for(int i = 0; i < scene->sceneObjects.size(); i++){
        std::string objectLabel = scene->sceneObjects[i].getObjectName();
        
        /*glm::vec3 objectPosition = scene->sceneObjects[i].transform.getPosition();
        std::string objectPositionText = std::string("\tPosition: X=") +  std::to_string(objectPosition.x) + " | Y=" + std::to_string(objectPosition.y) + " | Z=" + std::to_string(objectPosition.z);
        
        glm::vec3 objectRotation = scene->sceneObjects[i].transform.getRotation().getEulerAngles();
        std::string objectRotationText = std::string("\tRotation: X=") +  std::to_string(objectRotation.x) + " | Y=" + std::to_string(objectRotation.y) + " | Z=" + std::to_string(objectRotation.z);
        
        glm::vec3 objectScale = scene->sceneObjects[i].transform.getScale();
        std::string objectScaleText = std::string("\tScale: X=") +  std::to_string(objectPosition.x) + " | Y=" + std::to_string(objectPosition.y) + " | Z=" + std::to_string(objectPosition.z);*/
        
        //ImGui::Text(objectLabel.c_str());

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx(objectLabel.c_str(), flags);
        if(ImGui::IsItemClicked()){
            // Set to selected object
        }
        if(opened)
            ImGui::TreePop();
        
        // Implement drag and drop
    }

    ImGui::End();

    if(!windowOpened)
        viewListener->RemoveWindow(moduleIndex);
}