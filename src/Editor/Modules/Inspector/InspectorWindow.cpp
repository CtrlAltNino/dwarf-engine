#include "dpch.h"

#include "Editor/Modules/Inspector/InspectorWindow.h"

#include "Core/Asset/AssetDatabase.h"

#include <imgui_internal.h>
#include <math.h>

#define COMPONENT_PANEL_PADDING (8.0f)

namespace Dwarf {

    InspectorWindow::InspectorWindow(Ref<EditorModel> model, int id)
        :GuiModule(model, "Inspector", MODULE_TYPE::INSPECTOR, id), m_Scene(model->GetScene()){ }

    template<>
    void InspectorWindow::RenderComponent<IDComponent>(IDComponent* component){
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
        ImGui::Text(std::to_string(*component->ID).c_str());
    }

    template<>
    void InspectorWindow::RenderComponent<TagComponent>(TagComponent* component){
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
        //TagComponent* tc = &entity.GetComponent<TagComponent>();
        char* str0 = {(char*)component->Tag.c_str()};
        ImGui::InputText("Name", str0, sizeof(char) * 64);
        component->Tag = std::string(str0);
    }

    template<>
    void InspectorWindow::RenderComponent<TransformComponent>(TransformComponent* component){
        // Render Position
        // Render Rotation
        // Render Scale

        //ImGui::Text("Transform Component");
        //ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);

        //TransformComponent* transform = &entity.GetComponent<TransformComponent>();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
        ImGui::DragFloat3("Position", &component->position.x, 0.015f);
        static const float rad_2_deg = 180.0f / M_PI;
        static const float deg_2_rad = M_PI / 180.0f;
        //glm::vec3 rot = rad_2_deg * glm::eulerAngles(transform->rotation);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
        glm::vec3 rot = component->getEulerAngles();
        ImGui::DragFloat3("Rotation", &rot.x, 0.05f);
        component->rotation = rot;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
        ImGui::DragFloat3("Scale", &component->scale.x, 0.015f);
    }

    template<>
    void InspectorWindow::RenderComponent<LightComponent>(LightComponent* component){
        // Render dropdown for for light type
        // Render type depending variables
        //ImGui::Text("Light Component");
        //ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
        
        //LightComponent* lc = &entity.GetComponent<LightComponent>();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
        static int item_current = 0;
        ImGui::Combo("Light type", &item_current, LightComponent::LIGHT_TYPE_NAMES, IM_ARRAYSIZE(LightComponent::LIGHT_TYPE_NAMES));
        component->type = (LightComponent::LIGHT_TYPE)item_current;

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
        ImGui::ColorEdit3("Color", (float*)&component->lightColor.r, ImGuiColorEditFlags_None);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
        ImGui::DragFloat("Attenuation", &component->attenuation, 0.015f);
        
        if(component->type == LightComponent::LIGHT_TYPE::POINT_LIGHT){
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
            ImGui::DragFloat("Radius", &component->radius, 0.015f);
        }
        
        if(component->type == LightComponent::LIGHT_TYPE::SPOT_LIGHT){
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
            ImGui::SliderFloat("Opening Angle", &component->openingAngle, 0.0f, 180.0f);
        }
    }

    template<>
    void InspectorWindow::RenderComponent<MeshRendererComponent>(MeshRendererComponent* component){
        //ImGui::Text("Mesh Renderer Component");
        //ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
        
        // Render Mesh asset reference
        // Render Materials
        // Render check box for casting shadows
        //MeshRendererComponent* mrc = &entity.GetComponent<MeshRendererComponent>();
        
        // TODO: Slot for a mesh asset
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
        //char* meshAssetName = (char*)"Yeet";
        //ImGui::InputText("Mesh", meshAssetName, sizeof(meshAssetName), ImGuiInputTextFlags_ReadOnly);
        DwarfUI::AssetInput<MeshAsset>(component->mesh, "##meshAsset");

        //ImGuiInputTextFlags_ReadOnly

        if(component->mesh){
            int numMeshes = AssetDatabase::Retrieve<MeshAsset>(component->mesh)->GetAsset().m_Meshes.size();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
            ImGui::Text("Materials");

            if(component->materials.size() != numMeshes){
                component->materials.resize(numMeshes);
            }
            
            ImGui::Indent(10.0f);
            // TODO: Slots for material assets
            for(int i = 0; i < numMeshes; i++) {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
                //Ref<Material> currentMat = AssetDatabase::Retrieve<MaterialAsset>(component->materials[i])->GetAsset().m_Material;
                DwarfUI::AssetInput<MaterialAsset>(component->materials.at(i), (std::string("##materialAsset") + std::to_string(i)).c_str());
                //ImGui::Text((std::to_string(i) + std::string(" - ") + std::string(currentMat->GetName())).c_str());
            }
        }
    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<UnknownAsset>>(Ref<AssetReference<UnknownAsset>> asset){

    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<MaterialAsset>>(Ref<AssetReference<MaterialAsset>> asset){
        
    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<SceneAsset>>(Ref<AssetReference<SceneAsset>> asset){

    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<MeshAsset>>(Ref<AssetReference<MeshAsset>> asset){

    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<TextureAsset>>(Ref<AssetReference<TextureAsset>> asset){

    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<VertexShaderAsset>>(Ref<AssetReference<VertexShaderAsset>> asset){

    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<TesselationControlShaderAsset>>(Ref<AssetReference<TesselationControlShaderAsset>> asset){

    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<TesselationEvaluationShaderAsset>>(Ref<AssetReference<TesselationEvaluationShaderAsset>> asset){

    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<GeometryShaderAsset>>(Ref<AssetReference<GeometryShaderAsset>> asset){

    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<FragmentShaderAsset>>(Ref<AssetReference<FragmentShaderAsset>> asset){

    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<ComputeShaderAsset>>(Ref<AssetReference<ComputeShaderAsset>> asset){

    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<MetalShaderAsset>>(Ref<AssetReference<MetalShaderAsset>> asset){

    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<HlslShaderAsset>>(Ref<AssetReference<HlslShaderAsset>> asset){

    }

    void InspectorWindow::OnImGuiRender(){
        ImGuiWindowFlags window_flags = 0;

        //window_flags |= ImGuiWindowFlags_NoMove;
        //window_flags |= ImGuiWindowFlags_NoResize;
        //window_flags |= ImGuiWindowFlags_NoCollapse;
        //window_flags |= ImGuiWindowFlags_NoTitleBar;
        //window_flags |= ImGuiWindowFlags_MenuBar;

        //static bool b_open = true;
        //if (!ImGui::Begin((moduleLabel + "##" + std::to_string(index)).c_str(), NULL, window_flags))
        if (!ImGui::Begin(GetIdentifier().c_str(), &m_WindowOpened, window_flags))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            ImGui::PopStyleVar();
            return;
        }

        switch(m_Model->m_Selection.selectionType){
            case INSPECTOR_SELECTION_TYPE::ASSET:
            {
                // TODO Render Asset data
                //ImGui::Text(m_Model->m_Selection.assetPath.stem().string().c_str());
                //AssetDatabase::Retrieve(m_Model->m_Selection.assetPath);
                std::filesystem::path assetPath = m_Model->m_Selection.assetPath;
                switch(AssetDatabase::GetType(assetPath)){
                    case MODEL:
                        RenderComponent(AssetDatabase::Retrieve<MeshAsset>(assetPath));
                        break;
                    case TEXTURE:
                        RenderComponent(AssetDatabase::Retrieve<TextureAsset>(assetPath));
                        break;
                    case SCENE: 
                        RenderComponent(AssetDatabase::Retrieve<SceneAsset>(assetPath));
                        break;
                    case MATERIAL:
                        RenderComponent(AssetDatabase::Retrieve<MaterialAsset>(assetPath));
                        break;
                    case VERTEX_SHADER:
                        RenderComponent(AssetDatabase::Retrieve<VertexShaderAsset>(assetPath));
                        break;
                    case TESC_SHADER:
                        RenderComponent(AssetDatabase::Retrieve<TesselationControlShaderAsset>(assetPath));
                        break;
                    case TESE_SHADER:
                        RenderComponent(AssetDatabase::Retrieve<TesselationEvaluationShaderAsset>(assetPath));
                        break;
                    case GEOMETRY_SHADER:
                        RenderComponent(AssetDatabase::Retrieve<GeometryShaderAsset>(assetPath));
                        break;
                    case FRAGMENT_SHADER:
                        RenderComponent(AssetDatabase::Retrieve<FragmentShaderAsset>(assetPath));
                        break;
                    case HLSL_SHADER:
                        RenderComponent(AssetDatabase::Retrieve<HlslShaderAsset>(assetPath));
                        break;
                    case METAL_SHADER:
                        RenderComponent(AssetDatabase::Retrieve<MetalShaderAsset>(assetPath));
                        break;
                    case COMPUTE_SHADER:
                        RenderComponent(AssetDatabase::Retrieve<ComputeShaderAsset>(assetPath));
                        break;
                    case UNKNOWN:
                        RenderComponent(AssetDatabase::Retrieve<UnknownAsset>(assetPath));
                        break;
                    default: break;
                }
                break;
            }
            case INSPECTOR_SELECTION_TYPE::ENTITY:
                    static std::vector<Entity>* selectedEntities = &m_Model->m_Selection.selectedEntities;
                    if(selectedEntities->size() == 1) {
                        RenderComponents(selectedEntities->at(0));
                    }
                break;
            case INSPECTOR_SELECTION_TYPE::NONE:
                break;
        }

        ImGui::End();
    }

    void InspectorWindow::OnUpdate(double deltaTime){

    }

    void InspectorWindow::BeginComponent(const char* componentHeader){
        ImGui::BeginGroup();
        ImVec2 vec2 = ImGui::CalcTextSize(componentHeader);
        //ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING, ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING));
        ImGui::SetCursorPos(ImVec2(
            ImGui::GetContentRegionAvail().x / 2.0 - (vec2.x / 2.0),
            ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING));
        ImGui::Text(componentHeader);
        ImVec2 separatorMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + COMPONENT_PANEL_PADDING,
            ImGui::GetWindowPos().y + ImGui::GetCursorPos().y + COMPONENT_PANEL_PADDING / 2.0);
        ImVec2 separatorMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x - COMPONENT_PANEL_PADDING, separatorMin.y + 2);
        ImGui::GetWindowDrawList()->AddRectFilled(separatorMin,
            separatorMax, COL_BG_DIM);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING * 2);
    }

    void InspectorWindow::EndComponent(){
        ImGui::EndGroup();
        ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(),
            ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x, ImGui::GetItemRectMax().y + 2 * COMPONENT_PANEL_PADDING),
            IM_COL32(59, 66, 82, 255), 5.0f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 * COMPONENT_PANEL_PADDING);
        //ImGui::Separator();
    }

    void InspectorWindow::RenderComponents(Entity entity) {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->ChannelsSplit(2);
        
        if(entity.HasComponent<IDComponent>()){
            draw_list->ChannelsSetCurrent(1);
            BeginComponent("ID Component");
            RenderComponent(&entity.GetComponent<IDComponent>());
            draw_list->ChannelsSetCurrent(0);
            EndComponent();
        }
        

        if(entity.HasComponent<TagComponent>()){
            draw_list->ChannelsSetCurrent(1);
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            BeginComponent("Tag Component");
            RenderComponent(&entity.GetComponent<TagComponent>());
            draw_list->ChannelsSetCurrent(0);
            EndComponent();
        }

        if(entity.HasComponent<TransformComponent>()){
            draw_list->ChannelsSetCurrent(1);
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            BeginComponent("Transform Component");
            RenderComponent(&entity.GetComponent<TransformComponent>());
            draw_list->ChannelsSetCurrent(0);
            EndComponent();
        }

        if(entity.HasComponent<LightComponent>()){
            draw_list->ChannelsSetCurrent(1);
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            BeginComponent("Light Component");
            RenderComponent(&entity.GetComponent<LightComponent>());
            draw_list->ChannelsSetCurrent(0);
            EndComponent();
        }

        if(entity.HasComponent<MeshRendererComponent>()){
            draw_list->ChannelsSetCurrent(1);
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            BeginComponent("Mesh Renderer Component");
            RenderComponent(&entity.GetComponent<MeshRendererComponent>());
            draw_list->ChannelsSetCurrent(0);
            EndComponent();
        }
        draw_list->ChannelsMerge();
    }
}