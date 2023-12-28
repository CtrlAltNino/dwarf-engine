#include "dpch.h"

#include "Editor/Modules/Inspector/InspectorWindow.h"

#include "Core/Asset/AssetDatabase.h"
#include "Core/Scene/SceneUtilities.h"
#include "Editor/Modules/Inspector/AssetInspectorRenderer.h"

#include <imgui_internal.h>
#include <math.h>

#ifdef _WIN32
// #include "Platform/Direct3D12/D3D12Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
// #include "Platform/Vulkan/VulkanShader.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLShader.h"
// #include "Platform/Vulkan/VulkanShader.h"
#elif __APPLE__
// #include "Platform/Metal/MetalShader.h"
#endif

#define COMPONENT_PANEL_PADDING (8.0f)
#define ADD_BUTTON_WIDTH (40.0f)
#define UNIFORM_DELETE_BUTTON_WIDTH (80.0f)

namespace Dwarf
{

    InspectorWindow::InspectorWindow(Ref<EditorModel> model, int id)
        : GuiModule(model, "Inspector", MODULE_TYPE::INSPECTOR, id), m_Scene(model->GetScene())
    {
        AssetInspectorRenderer::Init(model);
    }

    template <>
    void InspectorWindow::RenderComponent<IDComponent>(IDComponent *component)
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
        ImGui::TextWrapped(std::to_string(*component->ID).c_str());
    }

    template <>
    void InspectorWindow::RenderComponent<NameComponent>(NameComponent *component)
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
        ImGui::TextWrapped("Name");
        ImGui::SameLine();
        char *str0 = {(char *)component->Name.c_str()};
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - COMPONENT_PANEL_PADDING);
        ImGui::InputText("##name_input", str0, sizeof(char) * 64);
        component->Name = std::string(str0);
        ImGui::PopItemWidth();
    }

    template <>
    void InspectorWindow::RenderComponent<TransformComponent>(TransformComponent *component)
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

        ImGui::TextWrapped("Position");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - COMPONENT_PANEL_PADDING);
        ImGui::DragFloat3("##transform_position", &component->position.x, 0.015f);
        ImGui::PopItemWidth();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

        ImGui::TextWrapped("Rotation");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

        static const float rad_2_deg = 180.0f / M_PI;
        static const float deg_2_rad = M_PI / 180.0f;
        glm::vec3 rot = component->getEulerAngles();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - COMPONENT_PANEL_PADDING);
        ImGui::DragFloat3("##transform_rotation", &rot.x, 0.05f);
        component->rotation = rot;
        ImGui::PopItemWidth();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

        ImGui::TextWrapped("Scale");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - COMPONENT_PANEL_PADDING);
        ImGui::DragFloat3("##transform_scale", &component->scale.x, 0.015f);
        ImGui::PopItemWidth();
    }

    template <>
    void InspectorWindow::RenderComponent<LightComponent>(LightComponent *component)
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

        static int item_current = 0;
        ImGui::TextWrapped("Light type");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - COMPONENT_PANEL_PADDING);
        ImGui::Combo("##light_type", &item_current, LightComponent::LIGHT_TYPE_NAMES, IM_ARRAYSIZE(LightComponent::LIGHT_TYPE_NAMES));
        ImGui::PopItemWidth();
        component->type = (LightComponent::LIGHT_TYPE)item_current;

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

        ImGui::TextWrapped("Color");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - COMPONENT_PANEL_PADDING);
        ImGui::ColorEdit3("##light_color", (float *)&component->lightColor.r, ImGuiColorEditFlags_None);
        ImGui::PopItemWidth();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

        ImGui::TextWrapped("Attenuation");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - COMPONENT_PANEL_PADDING);
        ImGui::DragFloat("##light_attenuation", &component->attenuation, 0.015f);
        ImGui::PopItemWidth();

        if (component->type == LightComponent::LIGHT_TYPE::POINT_LIGHT)
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
            ImGui::TextWrapped("Radius");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - COMPONENT_PANEL_PADDING);
            ImGui::DragFloat("##light_point_radius", &component->radius, 0.015f);
            ImGui::PopItemWidth();
        }

        if (component->type == LightComponent::LIGHT_TYPE::SPOT_LIGHT)
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
            ImGui::TextWrapped("Opening Angle");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - COMPONENT_PANEL_PADDING);
            ImGui::SliderFloat("##light_spot_angle", &component->openingAngle, 0.0f, 180.0f);
            ImGui::PopItemWidth();
        }
    }

    template <>
    void InspectorWindow::RenderComponent<MeshRendererComponent>(MeshRendererComponent *component)
    {
        // TODO: Slot for a mesh asset
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
        // char* meshAssetName = (char*)"Yeet";
        // ImGui::InputText("Mesh", meshAssetName, sizeof(meshAssetName), ImGuiInputTextFlags_ReadOnly);
        ImGui::TextWrapped("Model Asset");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - COMPONENT_PANEL_PADDING);
        DwarfUI::AssetInput<ModelAsset>(component->meshAsset, "##modelAsset");
        ImGui::PopItemWidth();

        if (component->meshAsset)
        {
            int numMaterials = 0;
            std::vector<Ref<Mesh>> meshes = AssetDatabase::Retrieve<ModelAsset>(component->meshAsset)->GetAsset()->m_Meshes;

            for (int i = 0; i < meshes.size(); i++)
            {
                if (meshes[i]->GetMaterialIndex() > numMaterials)
                {
                    numMaterials = meshes[i]->GetMaterialIndex();
                }
            }
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
            ImGui::TextWrapped("Materials");

            if (component->materialAssets.size() != numMaterials)
            {
                component->materialAssets.resize(numMaterials);
            }

            ImGui::Indent(16.0f);
            for (int i = 0; i < numMaterials; i++)
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
                ImGui::TextWrapped(std::to_string(i).c_str());
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - COMPONENT_PANEL_PADDING);
                DwarfUI::AssetInput<MaterialAsset>(component->materialAssets.at(i), (std::string("##materialAsset") + std::to_string(i)).c_str());
                ImGui::PopItemWidth();
            }
        }
    }

    void InspectorWindow::OnImGuiRender()
    {
        ImGuiWindowFlags window_flags = 0;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.0f, 14.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200.0f, 0));

        if (!ImGui::Begin(GetIdentifier().c_str(), &m_WindowOpened, window_flags))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            ImGui::PopStyleVar();
            return;
        }

        switch (m_Model->m_Selection.selectionType)
        {
        case INSPECTOR_SELECTION_TYPE::ASSET:
        {
            // TODO Render Asset data
            // ImGui::TextWrapped(m_Model->m_Selection.assetPath.stem().string().c_str());
            // AssetDatabase::Retrieve(m_Model->m_Selection.assetPath);
            // if(!m_Model->m_Selection.assetPath FileHandler::)
            std::filesystem::path assetPath = m_Model->m_Selection.assetPath;
            if (std::filesystem::is_regular_file(assetPath))
            {
                switch (AssetDatabase::GetType(assetPath))
                {
                case MODEL:
                    AssetInspectorRenderer::RenderAssetInspector(AssetDatabase::Retrieve<ModelAsset>(assetPath));
                    break;
                case TEXTURE:
                    AssetInspectorRenderer::RenderAssetInspector(AssetDatabase::Retrieve<TextureAsset>(assetPath));
                    break;
                case SCENE:
                    AssetInspectorRenderer::RenderAssetInspector(AssetDatabase::Retrieve<SceneAsset>(assetPath));
                    break;
                case MATERIAL:
                    AssetInspectorRenderer::RenderAssetInspector(AssetDatabase::Retrieve<MaterialAsset>(assetPath));
                    break;
                case VERTEX_SHADER:
                    AssetInspectorRenderer::RenderAssetInspector(AssetDatabase::Retrieve<VertexShaderAsset>(assetPath));
                    break;
                case TESC_SHADER:
                    AssetInspectorRenderer::RenderAssetInspector(AssetDatabase::Retrieve<TesselationControlShaderAsset>(assetPath));
                    break;
                case TESE_SHADER:
                    AssetInspectorRenderer::RenderAssetInspector(AssetDatabase::Retrieve<TesselationEvaluationShaderAsset>(assetPath));
                    break;
                case GEOMETRY_SHADER:
                    AssetInspectorRenderer::RenderAssetInspector(AssetDatabase::Retrieve<GeometryShaderAsset>(assetPath));
                    break;
                case FRAGMENT_SHADER:
                    AssetInspectorRenderer::RenderAssetInspector(AssetDatabase::Retrieve<FragmentShaderAsset>(assetPath));
                    break;
                case HLSL_SHADER:
                    AssetInspectorRenderer::RenderAssetInspector(AssetDatabase::Retrieve<HlslShaderAsset>(assetPath));
                    break;
                case COMPUTE_SHADER:
                    AssetInspectorRenderer::RenderAssetInspector(AssetDatabase::Retrieve<ComputeShaderAsset>(assetPath));
                    break;
                case UNKNOWN:
                    AssetInspectorRenderer::RenderAssetInspector(AssetDatabase::Retrieve<UnknownAsset>(assetPath));
                    break;
                default:
                    break;
                }
            }
            break;
        }
        case INSPECTOR_SELECTION_TYPE::ENTITY:
            static std::vector<Entity> *selectedEntities = &m_Model->m_Selection.selectedEntities;
            if (selectedEntities->size() == 1)
            {
                RenderComponents(selectedEntities->at(0));
            }
            break;
        case INSPECTOR_SELECTION_TYPE::NONE:
            break;
        }

        ImGui::End();

        ImGui::PopStyleVar(2);
    }

    void InspectorWindow::OnUpdate(double deltaTime)
    {
    }

    void InspectorWindow::BeginComponent(const char *componentHeader)
    {
        ImGui::BeginGroup();
        ImVec2 vec2 = ImGui::CalcTextSize(componentHeader);
        ImGui::SetCursorPos(ImVec2(
            ImGui::GetContentRegionAvail().x / 2.0 - (vec2.x / 2.0),
            ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING));
        ImGui::TextWrapped(componentHeader);
        ImVec2 separatorMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + COMPONENT_PANEL_PADDING,
                                     ImGui::GetWindowPos().y + ImGui::GetCursorPos().y + COMPONENT_PANEL_PADDING / 2.0);
        ImVec2 separatorMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x - COMPONENT_PANEL_PADDING, separatorMin.y + 2);
        ImGui::GetWindowDrawList()->AddRectFilled(separatorMin,
                                                  separatorMax, COL_BG_DIM);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING * 2);
    }

    void InspectorWindow::EndComponent()
    {
        ImGui::EndGroup();
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y),
                                                  ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x, ImGui::GetItemRectMax().y + 2 * COMPONENT_PANEL_PADDING),
                                                  IM_COL32(59, 66, 82, 255), 5.0f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 * COMPONENT_PANEL_PADDING);
    }

    void InspectorWindow::RenderComponents(Entity entity)
    {
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        draw_list->ChannelsSplit(2);

        /*if (entity.HasComponent<IDComponent>())
        {
            draw_list->ChannelsSetCurrent(1);
            BeginComponent("ID Component");
            RenderComponent(&entity.GetComponent<IDComponent>());
            draw_list->ChannelsSetCurrent(0);
            EndComponent();
        }*/

        if (entity.HasComponent<NameComponent>())
        {
            draw_list->ChannelsSetCurrent(1);
            ImDrawList *draw_list = ImGui::GetWindowDrawList();
            BeginComponent("Name");
            RenderComponent(&entity.GetComponent<NameComponent>());
            draw_list->ChannelsSetCurrent(0);
            EndComponent();
        }

        if (entity.HasComponent<TransformComponent>())
        {
            draw_list->ChannelsSetCurrent(1);
            ImDrawList *draw_list = ImGui::GetWindowDrawList();
            BeginComponent("Transform");
            RenderComponent(&entity.GetComponent<TransformComponent>());
            draw_list->ChannelsSetCurrent(0);
            EndComponent();
        }

        if (entity.HasComponent<LightComponent>())
        {
            draw_list->ChannelsSetCurrent(1);
            ImDrawList *draw_list = ImGui::GetWindowDrawList();
            BeginComponent("Light");
            RenderComponent(&entity.GetComponent<LightComponent>());
            draw_list->ChannelsSetCurrent(0);
            EndComponent();
        }

        if (entity.HasComponent<MeshRendererComponent>())
        {
            draw_list->ChannelsSetCurrent(1);
            ImDrawList *draw_list = ImGui::GetWindowDrawList();
            BeginComponent("Mesh Renderer");
            RenderComponent(&entity.GetComponent<MeshRendererComponent>());
            draw_list->ChannelsSetCurrent(0);
            EndComponent();
        }
        draw_list->ChannelsMerge();
    }

    std::string InspectorWindow::Serialize()
    {
        return "";
    }
}