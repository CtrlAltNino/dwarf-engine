#include "AssetInspectorRenderer.h"
#include "Core/Asset/AssetDatabase.h"
#include "Core/UI/DwarfUI.h"
#include "Core/Scene/SceneUtilities.h"
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
#include <Core/Rendering/Shader Parameters/BooleanShaderParameter.h>
#include <Core/Rendering/Shader Parameters/IntegerShaderParameter.h>
#include <Core/Rendering/Shader Parameters/FloatShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Tex2DShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Vec2ShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Vec3ShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Vec4ShaderParameter.h>

#define COMPONENT_PANEL_PADDING (8.0f)
#define ADD_BUTTON_WIDTH (40.0f)
#define UNIFORM_DELETE_BUTTON_WIDTH (80.0f)

namespace Dwarf
{
    Ref<EditorModel> AssetInspectorRenderer::s_Model = nullptr;

    void AssetInspectorRenderer::Init(Ref<EditorModel> model)
    {
        AssetInspectorRenderer::s_Model = model;
    }
    template <>
    void AssetInspectorRenderer::RenderAssetInspector<AssetReference<UnknownAsset>>(Ref<AssetReference<UnknownAsset>> asset)
    {
        ImGui::TextWrapped("File name: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().filename().string().c_str());

        ImGui::TextWrapped("Path: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().string().c_str());

        ImGui::TextWrapped("Type: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped("Unknown");
    }

    template <>
    void AssetInspectorRenderer::RenderAssetInspector<AssetReference<MaterialAsset>>(Ref<AssetReference<MaterialAsset>> asset)
    {
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        draw_list->ChannelsSplit(2);
        draw_list->ChannelsSetCurrent(1);

        ImGui::BeginChild("##inspector_child", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

        ImGui::TextWrapped("File name: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().filename().string().c_str());

        ImGui::TextWrapped("Path: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().string().c_str());

        ImGui::TextWrapped("Type: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped("Dwarf Engine Material");

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        if (ImGui::Button("Reimport"))
        {
            // TODO Fix
            AssetDatabase::Reimport(asset);
            ImGui::EndChild();
            return;
        }

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

        ImVec2 separatorMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + COMPONENT_PANEL_PADDING,
                                     ImGui::GetWindowPos().y + ImGui::GetCursorPos().y + COMPONENT_PANEL_PADDING / 2.0);
        ImVec2 separatorMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x - COMPONENT_PANEL_PADDING, separatorMin.y + 2);
        ImGui::GetWindowDrawList()->AddRectFilled(separatorMin,
                                                  separatorMax, COL_BG_DIM);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

        Ref<Material> mat = asset->GetAsset()->m_Material;

        ImGui::Checkbox("Transparent", &mat->m_Transparent);

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING);

        if (ImGui::CollapsingHeader("Shader"))
        {
            ImGui::Indent(15.0f);
            switch (Renderer::GetAPI())
            {
#ifdef _WIN32
            case GraphicsApi::D3D12:
                break;
            case GraphicsApi::Metal:
                break;
            case GraphicsApi::OpenGL:
            {
                OpenGLShader *shader = (OpenGLShader *)mat->m_Shader.get();
                ImGui::TextWrapped("Vertex Shader");
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
                DwarfUI::AssetInput<VertexShaderAsset>(shader->m_VertexShaderAsset, "##vertexShader");
                ImGui::PopItemWidth();

                if (shader->vert_log_length)
                {
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
                    if (ImGui::CollapsingHeader("Vertex Shader Error Log##vert"))
                    {
                        ImGui::TextWrapped(shader->vert_message);
                    }
                    ImGui::PopItemWidth();
                }

                ImGui::TextWrapped("Tessellation Control Shader");
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
                DwarfUI::AssetInput<TesselationControlShaderAsset>(shader->m_TessellationControlShaderAsset, "##tessellationControlShader");
                ImGui::PopItemWidth();

                if (shader->tesc_log_length > 0)
                {
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
                    if (ImGui::CollapsingHeader("Tessellation Control Shader Error Log##tesc"))
                    {
                        ImGui::TextWrapped(shader->tesc_message);
                    }
                    ImGui::PopItemWidth();
                }

                ImGui::TextWrapped("Tessellation Evaluation Shader");
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
                DwarfUI::AssetInput<TesselationEvaluationShaderAsset>(shader->m_TessellationEvaluationShaderAsset, "##tessellationEvaluationShader");
                ImGui::PopItemWidth();

                if (shader->tese_log_length > 0)
                {
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
                    if (ImGui::CollapsingHeader("Tessellation Evaluation Shader Error Log##tese"))
                    {
                        ImGui::TextWrapped(shader->tese_message);
                    }
                    ImGui::PopItemWidth();
                }

                ImGui::TextWrapped("Geometry Shader");
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
                DwarfUI::AssetInput<GeometryShaderAsset>(shader->m_GeometryShaderAsset, "##geometryShader");
                ImGui::PopItemWidth();

                if (shader->geom_log_length > 0)
                {
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
                    if (ImGui::CollapsingHeader("Geometry Shader Error Log##geom"))
                    {
                        ImGui::TextWrapped(shader->geom_message);
                    }
                    ImGui::PopItemWidth();
                }

                ImGui::TextWrapped("Fragment Shader");
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
                DwarfUI::AssetInput<FragmentShaderAsset>(shader->m_FragmentShaderAsset, "##fragmentShader");
                ImGui::PopItemWidth();

                if (shader->frag_log_length > 0)
                {
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);
                    if (ImGui::CollapsingHeader("Fragment Shader Error Log##frag"))
                    {
                        ImGui::TextWrapped(shader->frag_message);
                    }
                    ImGui::PopItemWidth();
                }
                break;
            }
            case GraphicsApi::Vulkan:
                break;
#elif __linux__
            case GraphicsApi::D3D12:
                break;
            case GraphicsApi::Metal:
                break;
            case GraphicsApi::OpenGL:
                break;
            case GraphicsApi::Vulkan:
                break;
#elif __APPLE__
            case GraphicsApi::D3D12:
                break;
            case GraphicsApi::Metal:
                break;
            case GraphicsApi::OpenGL:
                break;
            case GraphicsApi::Vulkan:
                break;
#endif
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

            if (ImGui::Button("Compile", ImVec2(60, 25)))
            {
                mat->m_Shader->Compile();
            }

            ImGui::SameLine();

            ImGui::TextWrapped(mat->m_Shader->IsCompiled() ? "Successfully Compiled" : "Couldn't compile");
            ImGui::Unindent(15.0f);
        }

        if (ImGui::CollapsingHeader("Parameters"))
        {
            ImGui::Indent(15.0f);
            // Listing all current parameters (Excluding reserved uniform names by the engine)
            int n = 0;
            for (auto i = mat->m_Parameters.begin(); i != mat->m_Parameters.end();)
            {
                if (std::find(std::begin(reservedParameterNames), std::end(reservedParameterNames), i->first) == std::end(reservedParameterNames))
                {
                    // Parameter UI specific to parameter type
                    switch (i->second->GetType())
                    {
                    case BOOLEAN:
                    {
                        Ref<BooleanShaderParameter> parameter = std::dynamic_pointer_cast<BooleanShaderParameter>(i->second);
                        ImGui::TextWrapped(i->first.c_str());
                        ImGui::SameLine();
                        ImGui::Checkbox((std::string("##boolean") + std::to_string(n++)).c_str(), &(parameter->m_Value));
                    }
                    break;
                    case INTEGER:
                    {
                        Ref<IntegerShaderParameter> parameter = std::dynamic_pointer_cast<IntegerShaderParameter>(i->second);
                        ImGui::TextWrapped(i->first.c_str());
                        ImGui::SameLine();
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - UNIFORM_DELETE_BUTTON_WIDTH);
                        ImGui::InputInt((std::string("##integer") + std::to_string(n++)).c_str(), &(parameter->m_Value));
                        ImGui::PopItemWidth();
                    }
                    break;
                    case FLOAT:
                    {
                        Ref<FloatShaderParameter> parameter = std::dynamic_pointer_cast<FloatShaderParameter>(i->second);
                        ImGui::TextWrapped(i->first.c_str());
                        ImGui::SameLine();
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - UNIFORM_DELETE_BUTTON_WIDTH);
                        ImGui::InputFloat((std::string("##float") + std::to_string(n++)).c_str(), &(parameter->m_Value));
                        ImGui::PopItemWidth();
                    }
                    break;
                    case TEX2D:
                    {
                        Ref<Tex2DShaderParameter> parameter = std::dynamic_pointer_cast<Tex2DShaderParameter>(i->second);
                        ImGui::TextWrapped(i->first.c_str());
                        ImGui::SameLine();
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - UNIFORM_DELETE_BUTTON_WIDTH);
                        DwarfUI::AssetInput<TextureAsset>(parameter->m_Value, (std::string("##textureAsset") + std::to_string(n++)).c_str());
                        ImGui::PopItemWidth();
                    }
                    break;
                    case VEC2:
                    {
                        Ref<Vec2ShaderParameter> parameter = std::dynamic_pointer_cast<Vec2ShaderParameter>(i->second);
                        ImGui::TextWrapped(i->first.c_str());
                        ImGui::SameLine();
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - UNIFORM_DELETE_BUTTON_WIDTH);
                        ImGui::InputFloat2((std::string("##vec2") + std::to_string(n++)).c_str(), &(parameter->m_Value.x));
                        ImGui::PopItemWidth();
                    }
                    break;
                    case VEC3:
                    {
                        Ref<Vec3ShaderParameter> parameter = std::dynamic_pointer_cast<Vec3ShaderParameter>(i->second);
                        ImGui::TextWrapped(i->first.c_str());
                        ImGui::SameLine();
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - UNIFORM_DELETE_BUTTON_WIDTH);
                        ImGui::InputFloat3((std::string("##vec3") + std::to_string(n++)).c_str(), &(parameter->m_Value.x));
                        ImGui::PopItemWidth();
                    }
                    break;
                    case VEC4:
                    {
                        Ref<Vec4ShaderParameter> parameter = std::dynamic_pointer_cast<Vec4ShaderParameter>(i->second);
                        ImGui::TextWrapped(i->first.c_str());
                        ImGui::SameLine();
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - UNIFORM_DELETE_BUTTON_WIDTH);
                        ImGui::ColorEdit4((std::string("##vec4") + std::to_string(n++)).c_str(), &(parameter->m_Value.x), ImGuiColorEditFlags_None);
                        ImGui::PopItemWidth();
                    }
                    break;
                    }

                    // Delete button for parameter
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - UNIFORM_DELETE_BUTTON_WIDTH);
                    if (ImGui::Button((std::string("Delete##") + std::to_string(n)).c_str(), ImVec2(UNIFORM_DELETE_BUTTON_WIDTH - 15.0f, 0)))
                    {
                        i = mat->m_Parameters.erase(i)--;
                    }
                    else
                    {
                        n++;
                    }
                }
                ++i;
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

            // Adding new parameter
            static char paramName[128] = "";
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - (100.0f + UNIFORM_DELETE_BUTTON_WIDTH + 15.0f));
            ImGui::InputTextWithHint("##paramName", "Name", paramName, IM_ARRAYSIZE(paramName), ImGuiInputTextFlags_CharsNoBlank);
            ImGui::PopItemWidth();
            ImGui::SameLine();
            static int selectedParameterType = 0;

            ImGui::PushItemWidth(100.0f);
            if (ImGui::BeginCombo("##paramType", parameterTypeNames[selectedParameterType]))
            {
                for (int i = 0; i < 7; i++)
                {
                    if (ImGui::Selectable(parameterTypeNames[i], i == selectedParameterType))
                    {
                        selectedParameterType = i;
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();

            ImGui::SameLine();
            if (ImGui::Button("Add##addParam", ImVec2(UNIFORM_DELETE_BUTTON_WIDTH - 15.0f, 0)) && std::strlen(paramName) > 0)
            {
                switch (selectedParameterType)
                {
                case ShaderParameterType::BOOLEAN:
                    mat->SetParameter(paramName, false);
                    break;
                case ShaderParameterType::INTEGER:
                    mat->SetParameter(paramName, 0);
                    break;
                case ShaderParameterType::FLOAT:
                    mat->SetParameter(paramName, 0.0f);
                    break;
                case ShaderParameterType::TEX2D:
                    mat->SetParameter(paramName, nullptr, ShaderParameterType::TEX2D);
                    break;
                case ShaderParameterType::VEC2:
                    mat->SetParameter(paramName, glm::vec2(0.0f, 0.0f));
                    break;
                case ShaderParameterType::VEC3:
                    mat->SetParameter(paramName, glm::vec3(0.0f, 0.0f, 0.0f));
                    break;
                case ShaderParameterType::VEC4:
                    mat->SetParameter(paramName, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                    break;
                }
                std::strcpy(paramName, "");
            }

            ImGui::Unindent(15.0f);
        }

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        if (ImGui::Button("Save changes", ImVec2(ImGui::GetContentRegionAvail().x / 2.0f, 50)))
        {
            MaterialSerializer::Serialize(*mat, asset->GetPath());
            AssetDatabase::Reimport(asset);
        }

        ImGui::SameLine();

        if (ImGui::Button("Generate parameters", ImVec2(ImGui::GetContentRegionAvail().x, 50)))
        {
            mat->GenerateShaderParameters();
        }

        draw_list->ChannelsSetCurrent(0);

        float endY = ImGui::GetItemRectMax().y;
        ImGui::EndChild();
        ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(),
                                                  ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x, endY + 2 * COMPONENT_PANEL_PADDING),
                                                  IM_COL32(59, 66, 82, 255), 5.0f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 * COMPONENT_PANEL_PADDING);
        draw_list->ChannelsMerge();
    }

    template <>
    void AssetInspectorRenderer::RenderAssetInspector<AssetReference<SceneAsset>>(Ref<AssetReference<SceneAsset>> asset)
    {
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        draw_list->ChannelsSplit(2);
        draw_list->ChannelsSetCurrent(1);

        ImGui::BeginChild("##inspector_child", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

        ImGui::TextWrapped("File name: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().filename().string().c_str());

        ImGui::TextWrapped("Path: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().string().c_str());

        ImGui::TextWrapped("Type: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped("Dwarf Engine Scene");

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        if (ImGui::Button("Reimport"))
        {
            AssetDatabase::Reimport(asset);
        }

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

        ImVec2 separatorMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + COMPONENT_PANEL_PADDING,
                                     ImGui::GetWindowPos().y + ImGui::GetCursorPos().y + COMPONENT_PANEL_PADDING / 2.0);
        ImVec2 separatorMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x - COMPONENT_PANEL_PADDING, separatorMin.y + 2);
        ImGui::GetWindowDrawList()->AddRectFilled(separatorMin,
                                                  separatorMax, COL_BG_DIM);

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

        if (ImGui::Button("Load scene") && FileHandler::CheckIfFileExists(asset->GetPath()))
        {
            AssetInspectorRenderer::s_Model->SetScene(SceneUtilities::LoadScene(asset->GetPath()));
        }

        draw_list->ChannelsSetCurrent(0);

        float endY = ImGui::GetItemRectMax().y;
        ImGui::EndChild();
        ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(),
                                                  ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x, endY + 2 * COMPONENT_PANEL_PADDING),
                                                  IM_COL32(59, 66, 82, 255), 5.0f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 * COMPONENT_PANEL_PADDING);
        draw_list->ChannelsMerge();
    }

    template <>
    void AssetInspectorRenderer::RenderAssetInspector<AssetReference<ModelAsset>>(Ref<AssetReference<ModelAsset>> asset)
    {
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        draw_list->ChannelsSplit(2);
        draw_list->ChannelsSetCurrent(1);

        ImGui::BeginChild("##inspector_child", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

        ImGui::TextWrapped("File name: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().filename().string().c_str());

        ImGui::TextWrapped("Path: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().string().c_str());

        ImGui::TextWrapped("Type: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped("Model");

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        if (ImGui::Button("Reimport"))
        {
            AssetDatabase::Reimport(asset);
        }

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

        ImVec2 separatorMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + COMPONENT_PANEL_PADDING,
                                     ImGui::GetWindowPos().y + ImGui::GetCursorPos().y + COMPONENT_PANEL_PADDING / 2.0);
        ImVec2 separatorMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x - COMPONENT_PANEL_PADDING, separatorMin.y + 2);
        ImGui::GetWindowDrawList()->AddRectFilled(separatorMin,
                                                  separatorMax, COL_BG_DIM);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

        ImGui::TextWrapped("TODO here will be settings for model import");

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

        ImGui::Text("Preview:");

        draw_list->ChannelsSetCurrent(0);

        float endY = ImGui::GetItemRectMax().y;
        ImGui::EndChild();
        ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(),
                                                  ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x, endY + 2 * COMPONENT_PANEL_PADDING),
                                                  IM_COL32(59, 66, 82, 255), 5.0f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 * COMPONENT_PANEL_PADDING);
        draw_list->ChannelsMerge();
    }

    template <>
    void AssetInspectorRenderer::RenderAssetInspector<AssetReference<TextureAsset>>(Ref<AssetReference<TextureAsset>> asset)
    {
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        draw_list->ChannelsSplit(2);
        draw_list->ChannelsSetCurrent(1);

        ImGui::BeginChild("##inspector_child", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

        ImGui::TextWrapped("File name: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().filename().string().c_str());

        ImGui::TextWrapped("Path: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().string().c_str());

        ImGui::TextWrapped("Type: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped("Texture");

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        if (ImGui::Button("Reimport"))
        {
            AssetDatabase::Reimport(asset);
        }

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

        ImVec2 separatorMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + COMPONENT_PANEL_PADDING,
                                     ImGui::GetWindowPos().y + ImGui::GetCursorPos().y + COMPONENT_PANEL_PADDING / 2.0);
        ImVec2 separatorMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x - COMPONENT_PANEL_PADDING, separatorMin.y + 2);
        ImGui::GetWindowDrawList()->AddRectFilled(separatorMin,
                                                  separatorMax, COL_BG_DIM);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

        float width = ImGui::GetContentRegionAvail().x;
        ImGui::TextWrapped("Preview:");

        ImTextureID texID = (ImTextureID)asset->GetAsset()->m_Texture->GetTextureID();
        ImGui::Image(texID, ImVec2(width, width));

        draw_list->ChannelsSetCurrent(0);

        float endY = ImGui::GetItemRectMax().y;
        ImGui::EndChild();
        ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(),
                                                  ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x, endY + 2 * COMPONENT_PANEL_PADDING),
                                                  IM_COL32(59, 66, 82, 255), 5.0f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 * COMPONENT_PANEL_PADDING);
        draw_list->ChannelsMerge();
    }

    template <>
    void AssetInspectorRenderer::RenderAssetInspector<AssetReference<VertexShaderAsset>>(Ref<AssetReference<VertexShaderAsset>> asset)
    {
        ImGui::TextWrapped("File name: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().filename().string().c_str());

        ImGui::TextWrapped("Path: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().string().c_str());

        ImGui::TextWrapped("Type: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped("Vertex shader");
    }

    template <>
    void AssetInspectorRenderer::RenderAssetInspector<AssetReference<TesselationControlShaderAsset>>(Ref<AssetReference<TesselationControlShaderAsset>> asset)
    {
        ImGui::TextWrapped("File name: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().filename().string().c_str());

        ImGui::TextWrapped("Path: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().string().c_str());

        ImGui::TextWrapped("Type: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped("Tesselation control shader");
    }

    template <>
    void AssetInspectorRenderer::RenderAssetInspector<AssetReference<TesselationEvaluationShaderAsset>>(Ref<AssetReference<TesselationEvaluationShaderAsset>> asset)
    {
        ImGui::TextWrapped("File name: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().filename().string().c_str());

        ImGui::TextWrapped("Path: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().string().c_str());

        ImGui::TextWrapped("Type: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped("Tesselation evaluation shader");
    }

    template <>
    void AssetInspectorRenderer::RenderAssetInspector<AssetReference<GeometryShaderAsset>>(Ref<AssetReference<GeometryShaderAsset>> asset)
    {
        ImGui::TextWrapped("File name: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().filename().string().c_str());

        ImGui::TextWrapped("Path: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().string().c_str());

        ImGui::TextWrapped("Type: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped("Geometry shader");
    }

    template <>
    void AssetInspectorRenderer::RenderAssetInspector<AssetReference<FragmentShaderAsset>>(Ref<AssetReference<FragmentShaderAsset>> asset)
    {
        ImGui::TextWrapped("File name: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().filename().string().c_str());

        ImGui::TextWrapped("Path: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().string().c_str());

        ImGui::TextWrapped("Type: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped("Fragment shader");
    }

    template <>
    void AssetInspectorRenderer::RenderAssetInspector<AssetReference<ComputeShaderAsset>>(Ref<AssetReference<ComputeShaderAsset>> asset)
    {
        ImGui::TextWrapped("File name: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().filename().string().c_str());

        ImGui::TextWrapped("Path: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().string().c_str());

        ImGui::TextWrapped("Type: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped("Compute shader");
    }

    template <>
    void AssetInspectorRenderer::RenderAssetInspector<AssetReference<HlslShaderAsset>>(Ref<AssetReference<HlslShaderAsset>> asset)
    {
        ImGui::TextWrapped("File name: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().filename().string().c_str());

        ImGui::TextWrapped("Path: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped(asset->GetPath().string().c_str());

        ImGui::TextWrapped("Type: ");
        ImGui::SameLine(0, 5.0f);
        ImGui::TextWrapped("Hlsl shader");
    }
}