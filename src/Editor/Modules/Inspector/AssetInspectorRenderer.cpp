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

#define COMPONENT_PANEL_PADDING (8.0f)
#define ADD_BUTTON_WIDTH (40.0f)
#define UNIFORM_DELETE_BUTTON_WIDTH (80.0f)

namespace Dwarf
{
    Ref<EditorModel> AssetInspectorRenderer::s_Model = nullptr;
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
        // ImVec2 vec2 = ImGui::CalcTextSize(asset->GetPath().filename().string().c_str());
        // ImGui::SetCursorPos(ImVec2(
        //     ImGui::GetContentRegionAvail().x / 2.0 - (vec2.x / 2.0),
        //     ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING));

        // ImGui::Text("Material");
        // ImVec2 separatorMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + COMPONENT_PANEL_PADDING,
        //                              ImGui::GetWindowPos().y + ImGui::GetCursorPos().y + COMPONENT_PANEL_PADDING / 2.0);
        // ImVec2 separatorMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x - COMPONENT_PANEL_PADDING, separatorMin.y + 2);
        // ImGui::GetWindowDrawList()->AddRectFilled(separatorMin,
        //                                           separatorMax, COL_BG_DIM);

        // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING * 2);
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
            ImGui::Indent(8.0f);
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
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                DwarfUI::AssetInput<VertexShaderAsset>(shader->m_VertexShaderAsset, "##vertexShader");
                ImGui::PopItemWidth();

                if (shader->vert_log_length)
                {
                    ImGui::TextWrapped(shader->vert_message);
                }

                ImGui::TextWrapped("Tessellation Control Shader");
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                DwarfUI::AssetInput<TesselationControlShaderAsset>(shader->m_TessellationControlShaderAsset, "##tessellationControlShader");
                ImGui::PopItemWidth();

                if (shader->tesc_log_length > 0)
                {
                    ImGui::TextWrapped(shader->tesc_message);
                }

                ImGui::TextWrapped("Tessellation Evaluation Shader");
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                DwarfUI::AssetInput<TesselationEvaluationShaderAsset>(shader->m_TessellationEvaluationShaderAsset, "##tessellationEvaluationShader");
                ImGui::PopItemWidth();

                if (shader->tese_log_length > 0)
                {
                    ImGui::TextWrapped(shader->tese_message);
                }

                ImGui::TextWrapped("Geometry Shader");
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                DwarfUI::AssetInput<GeometryShaderAsset>(shader->m_GeometryShaderAsset, "##geometryShader");
                ImGui::PopItemWidth();

                if (shader->geom_log_length)
                {
                    ImGui::TextWrapped(shader->geom_message);
                }

                ImGui::TextWrapped("Fragment Shader");
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                DwarfUI::AssetInput<FragmentShaderAsset>(shader->m_FragmentShaderAsset, "##fragmentShader");
                ImGui::PopItemWidth();

                if (shader->frag_log_length)
                {
                    ImGui::TextWrapped(shader->frag_message);
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

            if (ImGui::Button("Compile"))
            {
                mat->m_Shader->Compile();
            }

            ImGui::SameLine();

            ImGui::TextWrapped(mat->m_Shader->IsCompiled() ? "Successfully Compiled" : "Couldn't compile");
            ImGui::Unindent(8.0f);
        }

        /*if (ImGui::CollapsingHeader("Textures"))
        {
            ImGui::Indent(8.0f);
            int n = 0;
            for (auto i = mat->m_Uniforms.Textures.begin(); i != mat->m_Uniforms.Textures.end();)
            {
                ImGui::TextWrapped(i->first.c_str());
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - UNIFORM_DELETE_BUTTON_WIDTH);
                DwarfUI::AssetInput<TextureAsset>(i->second, (std::string("##textureAsset") + std::to_string(n++)).c_str());
                ImGui::PopItemWidth();
                ImGui::SameLine();
                if (ImGui::Button((std::string("Delete##texture_") + std::to_string(n)).c_str(), ImVec2(UNIFORM_DELETE_BUTTON_WIDTH, 0)))
                {
                    i = mat->m_Uniforms.Textures.erase(i);
                }
                else
                {
                    ++i;
                    n++;
                }
            }

            static char newTextureName[128] = "";

            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ADD_BUTTON_WIDTH);
            ImGui::InputTextWithHint("##textureName", "New Texture", newTextureName, IM_ARRAYSIZE(newTextureName), ImGuiInputTextFlags_CharsNoBlank);
            ImGui::PopItemWidth();
            ImGui::SameLine();
            if (ImGui::Button("Add##texture") && std::strlen(newTextureName) > 0)
            {
                mat->SetTexture(newTextureName, nullptr);
                std::strcpy(newTextureName, "");
            }
            ImGui::Unindent(8.0f);
        }

        if (ImGui::CollapsingHeader("Shader uniforms"))
        {
            ImGui::Indent(8.0f);
            if (ImGui::CollapsingHeader("Booleans"))
            {
                ImGui::Indent(8.0f);
                int n = 0;
                for (auto i = mat->m_Uniforms.Booleans.begin(); i != mat->m_Uniforms.Booleans.end();)
                {
                    ImGui::TextWrapped(i->first.c_str());
                    ImGui::SameLine();
                    ImGui::Checkbox((std::string("##boolean") + std::to_string(n++)).c_str(), &i->second);
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - UNIFORM_DELETE_BUTTON_WIDTH);
                    if (ImGui::Button((std::string("Delete##boolean_") + std::to_string(n)).c_str(), ImVec2(UNIFORM_DELETE_BUTTON_WIDTH, 0)))
                    {
                        i = mat->m_Uniforms.Booleans.erase(i);
                    }
                    else
                    {
                        ++i;
                        n++;
                    }
                }

                static char newBooleanName[128] = "";
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ADD_BUTTON_WIDTH);
                ImGui::InputTextWithHint("##boolName", "New boolean", newBooleanName, IM_ARRAYSIZE(newBooleanName), ImGuiInputTextFlags_CharsNoBlank);
                ImGui::PopItemWidth();
                ImGui::SameLine();
                if (ImGui::Button("Add##boolean") && std::strlen(newBooleanName) > 0)
                {
                    mat->SetUniform(newBooleanName, false);
                    std::strcpy(newBooleanName, "");
                }
                ImGui::Unindent(8.0f);
            }

            if (ImGui::CollapsingHeader("Integers"))
            {
                ImGui::Indent(8.0f);
                int n = 0;
                for (auto i = mat->m_Uniforms.Integers.begin(); i != mat->m_Uniforms.Integers.end();)
                {
                    ImGui::TextWrapped(i->first.c_str());
                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - UNIFORM_DELETE_BUTTON_WIDTH);
                    ImGui::InputInt((std::string("##integer") + std::to_string(n++)).c_str(), &i->second);
                    ImGui::PopItemWidth();
                    ImGui::SameLine();
                    if (ImGui::Button((std::string("Delete##integer_") + std::to_string(n)).c_str(), ImVec2(UNIFORM_DELETE_BUTTON_WIDTH, 0)))
                    {
                        i = mat->m_Uniforms.Integers.erase(i);
                    }
                    else
                    {
                        ++i;
                        n++;
                    }
                }

                static char newIntegerName[128] = "";
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ADD_BUTTON_WIDTH);
                ImGui::InputTextWithHint("##integerName", "New integer", newIntegerName, IM_ARRAYSIZE(newIntegerName), ImGuiInputTextFlags_CharsNoBlank);
                ImGui::PopItemWidth();
                ImGui::SameLine();
                if (ImGui::Button("Add##integer") && std::strlen(newIntegerName) > 0)
                {
                    mat->SetUniform(newIntegerName, 0);
                    std::strcpy(newIntegerName, "");
                }
                ImGui::Unindent(8.0f);
            }

            if (ImGui::CollapsingHeader("Floats"))
            {
                ImGui::Indent(8.0f);
                int n = 0;
                for (auto i = mat->m_Uniforms.Floats.begin(); i != mat->m_Uniforms.Floats.end();)
                {
                    ImGui::TextWrapped(i->first.c_str());
                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - UNIFORM_DELETE_BUTTON_WIDTH);
                    ImGui::InputFloat((std::string("##float") + std::to_string(n++)).c_str(), &i->second);
                    ImGui::PopItemWidth();
                    ImGui::SameLine();
                    if (ImGui::Button((std::string("Delete##float_") + std::to_string(n)).c_str(), ImVec2(UNIFORM_DELETE_BUTTON_WIDTH, 0)))
                    {
                        i = mat->m_Uniforms.Floats.erase(i);
                    }
                    else
                    {
                        ++i;
                        n++;
                    }
                }

                static char newFloatName[128] = "";
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ADD_BUTTON_WIDTH);
                ImGui::InputTextWithHint("##floatName", "New float", newFloatName, IM_ARRAYSIZE(newFloatName), ImGuiInputTextFlags_CharsNoBlank);
                ImGui::PopItemWidth();
                ImGui::SameLine();
                if (ImGui::Button("Add##float") && std::strlen(newFloatName) > 0)
                {
                    mat->SetUniform(newFloatName, 0.0f);
                    std::strcpy(newFloatName, "");
                }
                ImGui::Unindent(8.0f);
            }

            if (ImGui::CollapsingHeader("2D Uniforms"))
            {
                ImGui::Indent(8.0f);
                int n = 0;
                for (auto i = mat->m_Uniforms.Floats2D.begin(); i != mat->m_Uniforms.Floats2D.end();)
                {
                    ImGui::TextWrapped(i->first.c_str());
                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - UNIFORM_DELETE_BUTTON_WIDTH);
                    ImGui::InputFloat2((std::string("##vec2") + std::to_string(n++)).c_str(), &i->second.x);
                    ImGui::PopItemWidth();
                    ImGui::SameLine();
                    if (ImGui::Button((std::string("Delete##vec2_") + std::to_string(n)).c_str(), ImVec2(UNIFORM_DELETE_BUTTON_WIDTH, 0)))
                    {
                        i = mat->m_Uniforms.Floats2D.erase(i);
                    }
                    else
                    {
                        ++i;
                        n++;
                    }
                }

                static char newVec2Name[128] = "";
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ADD_BUTTON_WIDTH);
                ImGui::InputTextWithHint("##vec2Name", "New 2D variable", newVec2Name, IM_ARRAYSIZE(newVec2Name), ImGuiInputTextFlags_CharsNoBlank);
                ImGui::PopItemWidth();
                ImGui::SameLine();
                if (ImGui::Button("Add##vec2") && std::strlen(newVec2Name) > 0)
                {
                    mat->SetUniform(newVec2Name, {0, 0});
                    std::strcpy(newVec2Name, "");
                }
                ImGui::Unindent(8.0f);
            }

            if (ImGui::CollapsingHeader("3D Uniforms"))
            {
                ImGui::Indent(8.0f);
                int n = 0;
                for (auto i = mat->m_Uniforms.Floats3D.begin(); i != mat->m_Uniforms.Floats3D.end();)
                {
                    ImGui::TextWrapped(i->first.c_str());
                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - UNIFORM_DELETE_BUTTON_WIDTH);
                    ImGui::InputFloat3((std::string("##vec3") + std::to_string(n++)).c_str(), &i->second.x);
                    ImGui::PopItemWidth();
                    ImGui::SameLine();
                    if (ImGui::Button((std::string("Delete##vec3_") + std::to_string(n)).c_str(), ImVec2(UNIFORM_DELETE_BUTTON_WIDTH, 0)))
                    {
                        i = mat->m_Uniforms.Floats3D.erase(i);
                    }
                    else
                    {
                        ++i;
                        n++;
                    }
                }

                static char newVec3Name[128] = "";
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ADD_BUTTON_WIDTH);
                ImGui::InputTextWithHint("##vec3Name", "New 3D variable", newVec3Name, IM_ARRAYSIZE(newVec3Name), ImGuiInputTextFlags_CharsNoBlank);
                ImGui::PopItemWidth();
                ImGui::SameLine();
                if (ImGui::Button("Add##vec3") && std::strlen(newVec3Name) > 0)
                {
                    mat->SetUniform(newVec3Name, {0, 0, 0});
                    std::strcpy(newVec3Name, "");
                }
                ImGui::Unindent(8.0f);
            }

            if (ImGui::CollapsingHeader("4D Uniforms"))
            {
                ImGui::Indent(8.0f);
                int n = 0;
                for (auto i = mat->m_Uniforms.Floats4D.begin(); i != mat->m_Uniforms.Floats4D.end();)
                {
                    ImGui::TextWrapped(i->first.c_str());
                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - UNIFORM_DELETE_BUTTON_WIDTH);
                    ImGui::ColorEdit4((std::string("##vec4") + std::to_string(n++)).c_str(), &i->second.x, ImGuiColorEditFlags_None);
                    ImGui::PopItemWidth();
                    ImGui::SameLine();
                    if (ImGui::Button((std::string("Delete##vec4_") + std::to_string(n)).c_str(), ImVec2(UNIFORM_DELETE_BUTTON_WIDTH, 0)))
                    {
                        i = mat->m_Uniforms.Floats4D.erase(i);
                    }
                    else
                    {
                        ++i;
                        n++;
                    }
                }

                static char newVec4Name[128] = "";
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ADD_BUTTON_WIDTH);
                ImGui::InputTextWithHint("##vec4Name", "New color", newVec4Name, IM_ARRAYSIZE(newVec4Name), ImGuiInputTextFlags_CharsNoBlank);
                ImGui::PopItemWidth();
                ImGui::SameLine();
                if (ImGui::Button("Add##vec4") && std::strlen(newVec4Name) > 0)
                {
                    mat->SetUniform(newVec4Name, {1, 1, 1, 1});
                    std::strcpy(newVec4Name, "");
                }
                ImGui::Unindent(8.0f);
            }
            ImGui::Unindent(8.0f);
        }*/

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        if (ImGui::Button("Save changes", ImVec2(100, 50)))
        {
            MaterialSerializer::Serialize(*mat, asset->GetPath());
            AssetDatabase::Reimport(asset);
        }

        ImGui::SameLine();

        if (ImGui::Button("Generate Inputs", ImVec2(100, 50)))
        {
            mat->GenerateUniforms();
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