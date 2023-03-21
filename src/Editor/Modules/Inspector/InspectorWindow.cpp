#include "dpch.h"

#include "Editor/Modules/Inspector/InspectorWindow.h"

#include "Core/Asset/AssetDatabase.h"

#include <imgui_internal.h>
#include <math.h>

namespace Dwarf {

    InspectorWindow::InspectorWindow(Ref<EditorModel> model, int id)
        :GuiModule(model, "Inspector", MODULE_TYPE::INSPECTOR, id), m_Scene(model->GetScene()){ }
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
                // TODO Render Asset data
                ImGui::Text(m_Model->m_Selection.assetPath.stem().string().c_str());
                break;
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

    void InspectorWindow::RenderComponents(Entity entity) {
        if(entity.HasComponent<IDComponent>()){
            ImGui::Text(std::to_string(*entity.GetComponent<IDComponent>().ID).c_str());
            
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
            static const float rad_2_deg = 180.0f / M_PI;
            static const float deg_2_rad = M_PI / 180.0f;
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
            ImGui::Combo("Light type", &item_current, LightComponent::LIGHT_TYPE_NAMES, IM_ARRAYSIZE(LightComponent::LIGHT_TYPE_NAMES));
            lc->type = (LightComponent::LIGHT_TYPE)item_current;

            ImGui::ColorEdit3("Color", (float*)&lc->lightColor.r, ImGuiColorEditFlags_None);
            ImGui::DragFloat("Attenuation", &lc->attenuation, 0.015f);
            
            if(lc->type == LightComponent::LIGHT_TYPE::POINT_LIGHT){
                ImGui::DragFloat("Radius", &lc->radius, 0.015f);
            }
            
            if(lc->type == LightComponent::LIGHT_TYPE::SPOT_LIGHT){
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
                Ref<Material> currentMat = AssetDatabase::Retrieve<MaterialAsset>(mrc->materials[i])->GetAsset().m_Material;
                ImGui::Indent(10.0f);
                ImGui::Text((std::to_string(i) + std::string(" - ") + std::string(currentMat->GetName())).c_str());
            }

            ImGui::Separator();
        }
    }
}