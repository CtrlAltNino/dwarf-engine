#include "SceneGraphModule.h"

SceneGraphModule::SceneGraphModule(IViewListener *listener, Scene *scene, int index)
:IModule(listener, "Scene graph", MODULE_TYPE::SCENE_GRAPH, index), scene(scene){ }

void SceneGraphModule::DrawNode(entt::entity entity){
    Entity ent = Entity(entity, (EntityProvider*)scene);
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    //std::cout << entity->GetUID() << std::endl;
    //std::cout << entity->GetComponent<TransformComponent>().getPosition().x << std::endl; 
    std::string objectLabel = ent.GetComponent<TagComponent>().Tag;
    //std::string objectLabel = "";
    
    if(ent.GetComponent<TransformComponent>().children.size() != 0){
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent.GetHandle(), flags, objectLabel.c_str());
        if(ImGui::IsItemClicked()){
            // Set to selected object
        }
        
        if(opened){
            for(entt::entity child : ent.GetComponent<TransformComponent>().children){
                DrawNode(child);
            }
            ImGui::TreePop();
        }
    }else{
        //ImGui::Indent();
        ImGui::Selectable(("       " + objectLabel).c_str());
        if(ImGui::IsItemClicked()){
            // Set to selected object
        }
        //ImGui::Unindent();
    }
}

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

    /*for(int i = 0; i < scene->sceneObjects.size(); i++){
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx(objectLabel.c_str(), flags);
        if(ImGui::IsItemClicked()){
            // Set to selected object
        }
        if(opened)
            ImGui::TreePop();
        
        // Implement drag and drop
    }*/

    auto view = scene->m_Registry.view<TransformComponent, TagComponent, IDComponent>();
    //std::rbegin(view.);
    //auto it = std::make_reverse_iterator(view);
    //std::reverse(view.begin(), view.end());
    std::vector<entt::entity> container;
    
    for(auto entity : view){
    //for(auto entity = view.end(); entity != view.begin(); entity++){
        /*Entity ent = Entity(entity, (EntityProvider*)scene);
        if(!scene->m_Registry.valid(ent.GetComponent<TransformComponent>().parent)){
            DrawNode(entity);
        }*/
        container.push_back(entity);
    }

    for(auto entity = container.rbegin(); entity != container.rend(); ++entity){
        Entity ent = Entity(*entity, (EntityProvider*)scene);
        if(!scene->m_Registry.valid(ent.GetComponent<TransformComponent>().parent)){
            DrawNode(*entity);
        }
    }

    ImGui::End();

    if(!windowOpened)
        viewListener->RemoveWindow(moduleIndex);
}