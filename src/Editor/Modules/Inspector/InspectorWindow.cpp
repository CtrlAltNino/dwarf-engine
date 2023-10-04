#include "dpch.h"

#include "Editor/Modules/Inspector/InspectorWindow.h"

#include "Core/Asset/AssetDatabase.h"
#include "Core/Scene/SceneUtilities.h"

#include <imgui_internal.h>
#include <math.h>

#ifdef _WIN32
    //#include "Platform/Direct3D12/D3D12Shader.h"
    #include "Platform/OpenGL/OpenGLShader.h"
    //#include "Platform/Vulkan/VulkanShader.h"
#elif __linux__
    #include "Platform/OpenGL/OpenGLShader.h"
    //#include "Platform/Vulkan/VulkanShader.h"
#elif __APPLE__
    //#include "Platform/Metal/MetalShader.h"
#endif

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
        DwarfUI::AssetInput<ModelAsset>(component->meshAsset, "##modelAsset");

        //ImGuiInputTextFlags_ReadOnly

        if(component->meshAsset){
            int numMaterials = 0;
            std::vector<Ref<Mesh>> meshes = AssetDatabase::Retrieve<ModelAsset>(component->meshAsset)->GetAsset()->m_Meshes;

            for(int i = 0; i < meshes.size(); i++)
            {
                if(meshes[i]->GetMaterialIndex() > numMaterials)
                {
                    numMaterials = meshes[i]->GetMaterialIndex();
                }
            }
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
            ImGui::Text("Materials");

            if(component->materialAssets.size() != numMaterials){
                component->materialAssets.resize(numMaterials);
            }

            ImGui::Indent(10.0f);
            // TODO: Slots for material assets
            for(int i = 0; i < numMaterials; i++) {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
                //Ref<Material> currentMat = AssetDatabase::Retrieve<MaterialAsset>(component->materials[i])->GetAsset().m_Material;
                DwarfUI::AssetInput<MaterialAsset>(component->materialAssets.at(i), (std::string("##materialAsset") + std::to_string(i)).c_str());
                //ImGui::Text((std::to_string(i) + std::string(" - ") + std::string(currentMat->GetName())).c_str());
            }
        }
    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<UnknownAsset>>(Ref<AssetReference<UnknownAsset>> asset){
        ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
        ImGui::TextWrapped(asset->GetAsset()->m_FileContent.c_str());
        ImGui::PopTextWrapPos();
    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<MaterialAsset>>(Ref<AssetReference<MaterialAsset>> asset){
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->ChannelsSplit(2);
        draw_list->ChannelsSetCurrent(1);

        ImGui::BeginGroup();
        ImVec2 vec2 = ImGui::CalcTextSize(asset->GetPath().filename().string().c_str());
        //ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING, ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING));
        ImGui::SetCursorPos(ImVec2(
            ImGui::GetContentRegionAvail().x / 2.0 - (vec2.x / 2.0),
            ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING));
        ImGui::Text(asset->GetPath().filename().string().c_str());
        ImVec2 separatorMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + COMPONENT_PANEL_PADDING,
            ImGui::GetWindowPos().y + ImGui::GetCursorPos().y + COMPONENT_PANEL_PADDING / 2.0);
        ImVec2 separatorMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x - COMPONENT_PANEL_PADDING, separatorMin.y + 2);
        ImGui::GetWindowDrawList()->AddRectFilled(separatorMin,
            separatorMax, COL_BG_DIM);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING * 2);

        ImGui::Indent(14.0f);
        ImGui::Text("Dwarf Material");
        ImGui::Text(asset->GetPath().string().c_str());
        ImGui::Text(std::to_string((uint64_t)*asset->GetUID()).c_str());

        separatorMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + COMPONENT_PANEL_PADDING,
            ImGui::GetWindowPos().y + ImGui::GetCursorPos().y + COMPONENT_PANEL_PADDING / 2.0);
        separatorMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x - COMPONENT_PANEL_PADDING, separatorMin.y + 2);
        ImGui::GetWindowDrawList()->AddRectFilled(separatorMin,
            separatorMax, COL_BG_DIM);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING * 2);

        Ref<Material> mat = asset->GetAsset()->m_Material;

        ImGui::Checkbox("Transparent", &mat->m_Transparent);

        if(ImGui::CollapsingHeader("Shader")){
            ImGui::Indent(8.0f);
            switch(Renderer::GetAPI()){
#ifdef _WIN32
                case GraphicsApi::D3D12: break;
                case GraphicsApi::Metal: break;
                case GraphicsApi::OpenGL:
                    {
                        OpenGLShader* shader = (OpenGLShader*)mat->m_Shader.get();
                        ImGui::Text("Vertex Shader");
                        ImGui::SameLine();
                        DwarfUI::AssetInput<VertexShaderAsset>(shader->m_VertexShader, "##vertexShader");

                        if(shader->vert_log_length){
                            ImGui::Text(shader->vert_message);
                        }

                        ImGui::Text("Tessellation Control Shader");
                        ImGui::SameLine();
                        DwarfUI::AssetInput<TesselationControlShaderAsset>(shader->m_TessellationControlShader, "##tessellationControlShader");

                        if(shader->tesc_log_length > 0){
                            ImGui::Text(shader->tesc_message);
                        }

                        ImGui::Text("Tessellation Evaluation Shader");
                        ImGui::SameLine();
                        DwarfUI::AssetInput<TesselationEvaluationShaderAsset>(shader->m_TessellationEvaluationShader, "##tessellationEvaluationShader");

                        if(shader->tese_log_length > 0){
                            ImGui::Text(shader->tese_message);
                        }

                        ImGui::Text("Geometry Shader");
                        ImGui::SameLine();
                        DwarfUI::AssetInput<GeometryShaderAsset>(shader->m_GeometryShader, "##geometryShader");

                        if(shader->geom_log_length){
                            ImGui::Text(shader->geom_message);
                        }

                        ImGui::Text("Fragment Shader");
                        ImGui::SameLine();
                        DwarfUI::AssetInput<FragmentShaderAsset>(shader->m_FragmentShader, "##fragmentShader");

                        if(shader->frag_log_length){
                            ImGui::Text(shader->frag_message);
                        }
                        break;
                    }
                case GraphicsApi::Vulkan: break;
#elif __linux__
                case GraphicsApi::D3D12: break;
                case GraphicsApi::Metal: break;
                case GraphicsApi::OpenGL: break;
                case GraphicsApi::Vulkan: break;
#elif __APPLE__
                case GraphicsApi::D3D12: break;
                case GraphicsApi::Metal: break;
                case GraphicsApi::OpenGL: break;
                case GraphicsApi::Vulkan: break;
#endif
            }

            if(ImGui::Button("Compile")){
                mat->m_Shader->Compile();
            }

            ImGui::SameLine();

            ImGui::Text(mat->m_Shader->IsCompiled() ? "Successfully Compiled" : "Couldn't compile");
        }

        if(ImGui::CollapsingHeader("Textures")){
            ImGui::Indent(8.0f);
            int n = 0;
            for(auto i = mat->m_Textures.begin(); i != mat->m_Textures.end(); i++){
                ImGui::Text(i->first.c_str());
                ImGui::SameLine();
                DwarfUI::AssetInput<TextureAsset>(i->second, (std::string("##textureAsset") + std::to_string(n++)).c_str());
            }

            static char newTextureName[128] = "";
            ImGui::InputTextWithHint("##textureName", "New Texture", newTextureName, IM_ARRAYSIZE(newTextureName));
            ImGui::SameLine();
            if(ImGui::Button("Add")){
                mat->SetTexture(newTextureName, nullptr);
            }
            ImGui::Unindent(8.0f);
        }

        if(ImGui::CollapsingHeader("Shader uniforms")){
            ImGui::Indent(8.0f);
            if (ImGui::CollapsingHeader("Booleans")){
                ImGui::Indent(8.0f);
                int n = 0;
                for(auto i = mat->m_BoolUniforms.begin(); i != mat->m_BoolUniforms.end(); i++){
                    ImGui::Text(i->first.c_str());
                    ImGui::SameLine();
                    ImGui::Checkbox((std::string("##boolean") + std::to_string(n++)).c_str(), &i->second);
                    //DwarfUI::AssetInput<TextureAsset>(i->second, (std::string("##boolean") + std::to_string(n++)).c_str());
                }

                static char newBooleanName[128] = "";
                ImGui::InputTextWithHint("##boolName", "New boolean", newBooleanName, IM_ARRAYSIZE(newBooleanName));
                ImGui::SameLine();
                if(ImGui::Button("Add")){
                    mat->SetUniform(newBooleanName, false);
                }
                ImGui::Unindent(8.0f);
            }

            if (ImGui::CollapsingHeader("Integers")){
                ImGui::Indent(8.0f);
                int n = 0;
                for(auto i = mat->m_IntegerUniforms.begin(); i != mat->m_IntegerUniforms.end(); i++){
                    ImGui::Text(i->first.c_str());
                    ImGui::SameLine();
                    //ImGui::Checkbox((std::string("##integer") + std::to_string(n++)).c_str(), &i->second);
                    ImGui::InputInt((std::string("##integer") + std::to_string(n++)).c_str(), &i->second);
                    //DwarfUI::AssetInput<TextureAsset>(i->second, (std::string("##boolean") + std::to_string(n++)).c_str());
                }

                static char newIntegerName[128] = "";
                ImGui::InputTextWithHint("##integerName", "New integer", newIntegerName, IM_ARRAYSIZE(newIntegerName));
                ImGui::SameLine();
                if(ImGui::Button("Add")){
                    mat->SetUniform(newIntegerName, 0);
                }
                ImGui::Unindent(8.0f);
            }

            if (ImGui::CollapsingHeader("Floats")){
                ImGui::Indent(8.0f);
                int n = 0;
                for(auto i = mat->m_FloatUniforms.begin(); i != mat->m_FloatUniforms.end(); i++){
                    ImGui::Text(i->first.c_str());
                    ImGui::SameLine();
                    //ImGui::Checkbox((std::string("##integer") + std::to_string(n++)).c_str(), &i->second);
                    ImGui::InputFloat((std::string("##float") + std::to_string(n++)).c_str(), &i->second);
                    //DwarfUI::AssetInput<TextureAsset>(i->second, (std::string("##boolean") + std::to_string(n++)).c_str());
                }

                static char newFloatName[128] = "";
                ImGui::InputTextWithHint("##floatName", "New float", newFloatName, IM_ARRAYSIZE(newFloatName));
                ImGui::SameLine();
                if(ImGui::Button("Add")){
                    mat->SetUniform(newFloatName, 0.0f);
                }
                ImGui::Unindent(8.0f);
            }

            if (ImGui::CollapsingHeader("2D Uniforms")){
                ImGui::Indent(8.0f);
                int n = 0;
                for(auto i = mat->m_Vector2Uniforms.begin(); i != mat->m_Vector2Uniforms.end(); i++){
                    ImGui::Text(i->first.c_str());
                    ImGui::SameLine();
                    //ImGui::Checkbox((std::string("##integer") + std::to_string(n++)).c_str(), &i->second);
                    ImGui::InputFloat2((std::string("##vec2") + std::to_string(n++)).c_str(), &i->second.x);
                    //DwarfUI::AssetInput<TextureAsset>(i->second, (std::string("##boolean") + std::to_string(n++)).c_str());
                }

                static char newVec2Name[128] = "";
                ImGui::InputTextWithHint("##vec2Name", "New 2D variable", newVec2Name, IM_ARRAYSIZE(newVec2Name));
                ImGui::SameLine();
                if(ImGui::Button("Add")){
                    mat->SetUniform(newVec2Name, {0,0});
                }
                ImGui::Unindent(8.0f);
            }

            if (ImGui::CollapsingHeader("3D Uniforms")){
                ImGui::Indent(8.0f);
                int n = 0;
                for(auto i = mat->m_Vector3Uniforms.begin(); i != mat->m_Vector3Uniforms.end(); i++){
                    ImGui::Text(i->first.c_str());
                    ImGui::SameLine();
                    //ImGui::Checkbox((std::string("##integer") + std::to_string(n++)).c_str(), &i->second);
                    ImGui::InputFloat3((std::string("##vec3") + std::to_string(n++)).c_str(), &i->second.x);
                    //DwarfUI::AssetInput<TextureAsset>(i->second, (std::string("##boolean") + std::to_string(n++)).c_str());
                }

                static char newVec3Name[128] = "";
                ImGui::InputTextWithHint("##vec3Name", "New 3D variable", newVec3Name, IM_ARRAYSIZE(newVec3Name));
                ImGui::SameLine();
                if(ImGui::Button("Add")){
                    mat->SetUniform(newVec3Name, {0,0,0});
                }
                ImGui::Unindent(8.0f);
            }

            if (ImGui::CollapsingHeader("4D Uniforms")){
                ImGui::Indent(8.0f);
                int n = 0;
                for(auto i = mat->m_Vector4Uniforms.begin(); i != mat->m_Vector4Uniforms.end(); i++){
                    ImGui::Text(i->first.c_str());
                    ImGui::SameLine();
                    //ImGui::Checkbox((std::string("##integer") + std::to_string(n++)).c_str(), &i->second);
                    //ImGui::InputFloat4((std::string("##vec4") + std::to_string(n++)).c_str(), &i->second.x);
                    ImGui::ColorPicker4((std::string("##vec4") + std::to_string(n++)).c_str(), &i->second.x);
                    //DwarfUI::AssetInput<TextureAsset>(i->second, (std::string("##boolean") + std::to_string(n++)).c_str());
                }

                static char newVec4Name[128] = "";
                ImGui::InputTextWithHint("##vec4Name", "New color", newVec4Name, IM_ARRAYSIZE(newVec4Name));
                ImGui::SameLine();
                if(ImGui::Button("Add")){
                    mat->SetUniform(newVec4Name, {1,1,1,1});
                }
                ImGui::Unindent(8.0f);
            }
            ImGui::Unindent(8.0f);
        }

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        if(ImGui::Button("Save changes", ImVec2(100, 50))){
            MaterialSerializer::Serialize(*mat, asset->GetPath());
            AssetDatabase::Reimport(asset);
        }

        ImGui::SameLine();

        if(ImGui::Button("Reimport", ImVec2(100, 50))){
            AssetDatabase::Reimport(asset);
        }

        ImGui::SameLine();

        if(ImGui::Button("Generate Inputs", ImVec2(100, 50))){
            mat->GenerateShaderInputs();
        }

        draw_list->ChannelsSetCurrent(0);

        ImGui::EndGroup();
        ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(),
            ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x, ImGui::GetItemRectMax().y + 2 * COMPONENT_PANEL_PADDING),
            IM_COL32(59, 66, 82, 255), 5.0f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 * COMPONENT_PANEL_PADDING);
        draw_list->ChannelsMerge();
    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<SceneAsset>>(Ref<AssetReference<SceneAsset>> asset){
        ImGui::Text(asset->GetPath().filename().string().c_str());
        ImGui::Text("Dwarf scene");
        if(ImGui::Button("Load scene") && FileHandler::CheckIfFileExists(asset->GetPath())){
            m_Model->SetScene(SceneUtilities::LoadScene(asset->GetPath()));
        }

        if(ImGui::Button("Reimport")){
            AssetDatabase::Reimport(asset);
        }
    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<ModelAsset>>(Ref<AssetReference<ModelAsset>> asset){
        ImGui::Text(asset->GetPath().filename().string().c_str());
        ImGui::Text("Model");

        if(ImGui::Button("Reimport")){
            AssetDatabase::Reimport(asset);
        }
    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<TextureAsset>>(Ref<AssetReference<TextureAsset>> asset){
        ImGui::Text(asset->GetPath().filename().string().c_str());
        ImGui::Text("Texture");

        ImTextureID texID = (ImTextureID)asset->GetAsset()->m_Texture->GetTextureID();
        ImGui::Image(texID, ImVec2(256, 256));

        if(ImGui::Button("Reimport")){
            AssetDatabase::Reimport(asset);
        }
    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<VertexShaderAsset>>(Ref<AssetReference<VertexShaderAsset>> asset){
        ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
        ImGui::TextWrapped(asset->GetAsset()->m_FileContent.c_str());
        ImGui::PopTextWrapPos();
    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<TesselationControlShaderAsset>>(Ref<AssetReference<TesselationControlShaderAsset>> asset){
        ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
        ImGui::TextWrapped(asset->GetAsset()->m_FileContent.c_str());
        ImGui::PopTextWrapPos();
    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<TesselationEvaluationShaderAsset>>(Ref<AssetReference<TesselationEvaluationShaderAsset>> asset){
        ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
        ImGui::TextWrapped(asset->GetAsset()->m_FileContent.c_str());
        ImGui::PopTextWrapPos();
    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<GeometryShaderAsset>>(Ref<AssetReference<GeometryShaderAsset>> asset){
        ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
        ImGui::TextWrapped(asset->GetAsset()->m_FileContent.c_str());
        ImGui::PopTextWrapPos();
    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<FragmentShaderAsset>>(Ref<AssetReference<FragmentShaderAsset>> asset){
        ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
        ImGui::TextWrapped(asset->GetAsset()->m_FileContent.c_str());
        ImGui::PopTextWrapPos();
    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<ComputeShaderAsset>>(Ref<AssetReference<ComputeShaderAsset>> asset){
        ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
        ImGui::TextWrapped(asset->GetAsset()->m_FileContent.c_str());
        ImGui::PopTextWrapPos();
    }

    template<>
    void InspectorWindow::RenderComponent<AssetReference<HlslShaderAsset>>(Ref<AssetReference<HlslShaderAsset>> asset){
        ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
        ImGui::TextWrapped(asset->GetAsset()->m_FileContent.c_str());
        ImGui::PopTextWrapPos();
    }

    void InspectorWindow::OnImGuiRender(){
        ImGuiWindowFlags window_flags = 0;

        //window_flags |= ImGuiWindowFlags_NoMove;
        //window_flags |= ImGuiWindowFlags_NoResize;
        //window_flags |= ImGuiWindowFlags_NoCollapse;
        //window_flags |= ImGuiWindowFlags_NoTitleBar;
        //window_flags |= ImGuiWindowFlags_MenuBar;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.0f, 14.0f));

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
                //if(!m_Model->m_Selection.assetPath FileHandler::)
                std::filesystem::path assetPath = m_Model->m_Selection.assetPath;
                if(std::filesystem::is_regular_file(assetPath)){
                    switch(AssetDatabase::GetType(assetPath)){
                        case MODEL:
                            RenderComponent(AssetDatabase::Retrieve<ModelAsset>(assetPath));
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
                        case COMPUTE_SHADER:
                            RenderComponent(AssetDatabase::Retrieve<ComputeShaderAsset>(assetPath));
                            break;
                        case UNKNOWN:
                            RenderComponent(AssetDatabase::Retrieve<UnknownAsset>(assetPath));
                            break;
                        default: break;
                    }
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

        ImGui::PopStyleVar();
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
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y),
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

    std::string InspectorWindow::Serialize(){
        return "";
    }
}