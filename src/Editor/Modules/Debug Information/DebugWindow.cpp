#include "dpch.h"

#include <imgui_internal.h>

#include "Editor/Modules/Debug Information/DebugWindow.h"
#include "Core/Asset/AssetDatabase.h"
#include "Core/Asset/AssetComponents.h"
#include "Core/GenericComponents.h"

namespace Dwarf
{

    DebugWindow::DebugWindow(Ref<EditorModel> model, int id)
        : GuiModule(model, "Debug", MODULE_TYPE::DEBUG, id) {}

    void DebugWindow::OnUpdate(double deltaTime)
    {
    }

    void DebugWindow::OnImGuiRender()
    {
        ImGuiWindowFlags window_flags = 0;

        // window_flags |= ImGuiWindowFlags_NoMove;
        // window_flags |= ImGuiWindowFlags_NoResize;
        // window_flags |= ImGuiWindowFlags_NoCollapse;
        // window_flags |= ImGuiWindowFlags_NoTitleBar;
        // window_flags |= ImGuiWindowFlags_MenuBar;

        // static bool b_open = true;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(500, 500));
        // if (!ImGui::Begin((moduleLabel + "##" + std::to_string(index)).c_str(), NULL, window_flags))
        if (!ImGui::Begin(GetIdentifier().c_str(), &m_WindowOpened, window_flags))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            ImGui::PopStyleVar();
            return;
        }

        // ImGui::ShowDemoWindow();
        if (ImGui::CollapsingHeader("Asset Database"))
        {
            ImGui::Text("Listing all imported assets and their UID's");

            auto materialView = AssetDatabase::s_Registry->view<MaterialAsset, PathComponent, TagComponent, IDComponent>();
            auto modelView = AssetDatabase::s_Registry->view<ModelAsset, PathComponent, TagComponent, IDComponent>();
            auto sceneView = AssetDatabase::s_Registry->view<SceneAsset, PathComponent, TagComponent, IDComponent>();
            auto vertexShaderView = AssetDatabase::s_Registry->view<VertexShaderAsset, PathComponent, TagComponent, IDComponent>();
            auto fragmentShaderView = AssetDatabase::s_Registry->view<FragmentShaderAsset, PathComponent, TagComponent, IDComponent>();
            auto geometryShaderView = AssetDatabase::s_Registry->view<GeometryShaderAsset, PathComponent, TagComponent, IDComponent>();
            auto computeShaderView = AssetDatabase::s_Registry->view<ComputeShaderAsset, PathComponent, TagComponent, IDComponent>();
            auto tesselationControlShaderView = AssetDatabase::s_Registry->view<TesselationControlShaderAsset, PathComponent, TagComponent, IDComponent>();
            auto tesselationEvaluationShaderView = AssetDatabase::s_Registry->view<TesselationEvaluationShaderAsset, PathComponent, TagComponent, IDComponent>();
            auto textureView = AssetDatabase::s_Registry->view<TextureAsset, PathComponent, TagComponent, IDComponent>();

            if (ImGui::TreeNode("Materials##1"))
            {
                for (auto entity : materialView)
                {
                    auto [path, tag, id] = materialView.get<PathComponent, TagComponent, IDComponent>(entity);
                    ImGui::Text("Name");
                    ImGui::Text(tag.Tag.c_str());
                    ImGui::Text("Path");
                    ImGui::Text(path.Path.string().c_str());
                    ImGui::Text("ID");
                    ImGui::Text(std::to_string((uint64_t) * (id.ID)).c_str());
                    ImGui::Separator();
                    ImGui::Separator();
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Models##1"))
            {
                for (auto entity : modelView)
                {
                    auto [path, tag, id] = modelView.get<PathComponent, TagComponent, IDComponent>(entity);
                    ImGui::Text("Name");
                    ImGui::Text(tag.Tag.c_str());
                    ImGui::Text("Path");
                    ImGui::Text(path.Path.string().c_str());
                    ImGui::Text("ID");
                    ImGui::Text(std::to_string((uint64_t) * (id.ID)).c_str());
                    ImGui::Separator();
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Scenes##1"))
            {
                for (auto entity : sceneView)
                {
                    auto [path, tag, id] = sceneView.get<PathComponent, TagComponent, IDComponent>(entity);
                    ImGui::Text("Name");
                    ImGui::Text(tag.Tag.c_str());
                    ImGui::Text("Path");
                    ImGui::Text(path.Path.string().c_str());
                    ImGui::Text("ID");
                    ImGui::Text(std::to_string((uint64_t) * (id.ID)).c_str());
                    ImGui::Separator();
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Shaders##1"))
            {
                if (ImGui::TreeNode("Vertex Shaders##1"))
                {
                    for (auto entity : vertexShaderView)
                    {
                        auto [path, tag, id] = vertexShaderView.get<PathComponent, TagComponent, IDComponent>(entity);
                        ImGui::Text("Name");
                        ImGui::Text(tag.Tag.c_str());
                        ImGui::Text("Path");
                        ImGui::Text(path.Path.string().c_str());
                        ImGui::Text("ID");
                        ImGui::Text(std::to_string((uint64_t) * (id.ID)).c_str());
                        ImGui::Separator();
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Fragment Shaders##1"))
                {
                    for (auto entity : fragmentShaderView)
                    {
                        auto [path, tag, id] = fragmentShaderView.get<PathComponent, TagComponent, IDComponent>(entity);
                        ImGui::Text("Name");
                        ImGui::Text(tag.Tag.c_str());
                        ImGui::Text("Path");
                        ImGui::Text(path.Path.string().c_str());
                        ImGui::Text("ID");
                        ImGui::Text(std::to_string((uint64_t) * (id.ID)).c_str());
                        ImGui::Separator();
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Geometry Shaders##1"))
                {
                    for (auto entity : geometryShaderView)
                    {
                        auto [path, tag, id] = geometryShaderView.get<PathComponent, TagComponent, IDComponent>(entity);
                        ImGui::Text("Name");
                        ImGui::Text(tag.Tag.c_str());
                        ImGui::Text("Path");
                        ImGui::Text(path.Path.string().c_str());
                        ImGui::Text("ID");
                        ImGui::Text(std::to_string((uint64_t) * (id.ID)).c_str());
                        ImGui::Separator();
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Compute Shaders##1"))
                {
                    for (auto entity : computeShaderView)
                    {
                        auto [path, tag, id] = computeShaderView.get<PathComponent, TagComponent, IDComponent>(entity);
                        ImGui::Text("Name");
                        ImGui::Text(tag.Tag.c_str());
                        ImGui::Text("Path");
                        ImGui::Text(path.Path.string().c_str());
                        ImGui::Text("ID");
                        ImGui::Text(std::to_string((uint64_t) * (id.ID)).c_str());
                        ImGui::Separator();
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Tesselation Control Shaders##1"))
                {
                    for (auto entity : tesselationControlShaderView)
                    {
                        auto [path, tag, id] = tesselationControlShaderView.get<PathComponent, TagComponent, IDComponent>(entity);
                        ImGui::Text("Name");
                        ImGui::Text(tag.Tag.c_str());
                        ImGui::Text("Path");
                        ImGui::Text(path.Path.string().c_str());
                        ImGui::Text("ID");
                        ImGui::Text(std::to_string((uint64_t) * (id.ID)).c_str());
                        ImGui::Separator();
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Tesselation Evaluation Shaders##1"))
                {
                    for (auto entity : tesselationEvaluationShaderView)
                    {
                        auto [path, tag, id] = tesselationEvaluationShaderView.get<PathComponent, TagComponent, IDComponent>(entity);
                        ImGui::Text("Name");
                        ImGui::Text(tag.Tag.c_str());
                        ImGui::Text("Path");
                        ImGui::Text(path.Path.string().c_str());
                        ImGui::Text("ID");
                        ImGui::Text(std::to_string((uint64_t) * (id.ID)).c_str());
                        ImGui::Separator();
                    }
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Textures##1"))
            {
                for (auto entity : textureView)
                {
                    auto [path, tag, id] = textureView.get<PathComponent, TagComponent, IDComponent>(entity);
                    ImGui::Text("Name");
                    ImGui::Text(tag.Tag.c_str());
                    ImGui::Text("Path");
                    ImGui::Text(path.Path.string().c_str());
                    ImGui::Text("ID");
                    ImGui::Text(std::to_string((uint64_t) * (id.ID)).c_str());
                    ImGui::Separator();
                }
                ImGui::TreePop();
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();
    }

    std::string DebugWindow::Serialize()
    {
        return "";
    }
}