#include "Editor/Modules/Debug Console/DebugModule.h"

namespace Dwarf {

    DebugModule::DebugModule(IViewListener *listener, AssetDatabase* assetDatabase, int id)
        :IModule(listener, "Debug", MODULE_TYPE::DEBUG, id),
        assetDatabase(assetDatabase) { }

    void DebugModule::RenderModuleWindow(){
        ImGuiWindowFlags window_flags = 0;

        //window_flags |= ImGuiWindowFlags_NoMove;
        //window_flags |= ImGuiWindowFlags_NoResize;
        //window_flags |= ImGuiWindowFlags_NoCollapse;
        //window_flags |= ImGuiWindowFlags_NoTitleBar;
        //window_flags |= ImGuiWindowFlags_MenuBar;

        //static bool b_open = true;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(500,500));
        //if (!ImGui::Begin((moduleLabel + "##" + std::to_string(index)).c_str(), NULL, window_flags))
        if (!ImGui::Begin(GetIdentifier().c_str(), &windowOpened, window_flags))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            ImGui::PopStyleVar();
            return;
        }

        //ImGui::ShowDemoWindow();
        if (ImGui::CollapsingHeader("Asset Database"))
        {
            ImGui::Text("Listing all imported assets and their UID's");

            auto materialView = assetDatabase->m_Registry.view<MaterialAsset>();
            auto modelView = assetDatabase->m_Registry.view<MeshAsset>();
            auto sceneView = assetDatabase->m_Registry.view<SceneAsset>();
            auto vertexShaderView = assetDatabase->m_Registry.view<VertexShaderAsset>();
            auto fragmentShaderView = assetDatabase->m_Registry.view<FragmentShaderAsset>();
            auto geometryShaderView = assetDatabase->m_Registry.view<GeometryShaderAsset>();
            auto computeShaderView = assetDatabase->m_Registry.view<ComputeShaderAsset>();
            auto tesselationControlShaderView = assetDatabase->m_Registry.view<TesselationControlShaderAsset>();
            auto tesselationEvaluationShaderView = assetDatabase->m_Registry.view<TesselationEvaluationShaderAsset>();
            auto textureView = assetDatabase->m_Registry.view<TextureAsset>();

            if (ImGui::TreeNode("Materials##1"))
            {
                for(auto entity: materialView) {
                    auto &mat = materialView.get<MaterialAsset>(entity);
                    ImGui::Text("Path");
                    ImGui::Text(mat.path.string().c_str());
                    ImGui::Separator();

                    ImGui::Text("Loaded:");
                    ImGui::SameLine();
                    ImGui::Text(mat.loaded ? "Yes" : "No");
                    ImGui::Separator();
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Models##1"))
            {
                for(auto entity: modelView) {
                    auto &mesh = modelView.get<MeshAsset>(entity);
                    ImGui::Text("Path");
                    ImGui::Text(mesh.path.string().c_str());

                    ImGui::Text("Loaded:");
                    ImGui::SameLine();
                    ImGui::Text(mesh.loaded ? "Yes" : "No");
                    ImGui::Separator();
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Scenes##1"))
            {
                for(auto entity: sceneView) {
                    auto &scene = sceneView.get<SceneAsset>(entity);
                    ImGui::Text("Path");
                    ImGui::Text(scene.path.string().c_str());
                    ImGui::Separator();
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Shaders##1"))
            {
                if (ImGui::TreeNode("Vertex Shaders##1"))
                {
                    for(auto entity: vertexShaderView) {
                        auto &vertexShader = vertexShaderView.get<VertexShaderAsset>(entity);
                        ImGui::Text("Path");
                        ImGui::Text(vertexShader.path.string().c_str());
                        ImGui::Separator();
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Fragment Shaders##1"))
                {
                    for(auto entity: fragmentShaderView) {
                        auto &fragmentShader = fragmentShaderView.get<FragmentShaderAsset>(entity);
                        ImGui::Text("Path");
                        ImGui::Text(fragmentShader.path.string().c_str());
                        ImGui::Separator();
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Geometry Shaders##1"))
                {
                    for(auto entity: geometryShaderView) {
                        auto &geometryShader = geometryShaderView.get<GeometryShaderAsset>(entity);
                        ImGui::Text("Path");
                        ImGui::Text(geometryShader.path.string().c_str());
                        ImGui::Separator();
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Compute Shaders##1"))
                {
                    for(auto entity: computeShaderView) {
                        auto &computeShader = computeShaderView.get<ComputeShaderAsset>(entity);
                        ImGui::Text("Path");
                        ImGui::Text(computeShader.path.string().c_str());
                        ImGui::Separator();
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Tesselation Control Shaders##1"))
                {
                    for(auto entity: tesselationControlShaderView) {
                        auto &tesselationControlShader = tesselationControlShaderView.get<TesselationControlShaderAsset>(entity);
                        ImGui::Text("Path");
                        ImGui::Text(tesselationControlShader.path.string().c_str());
                        ImGui::Separator();
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Tesselation Evaluation Shaders##1"))
                {
                    for(auto entity: tesselationEvaluationShaderView) {
                        auto &tesselationEvaluationShader = tesselationEvaluationShaderView.get<TesselationEvaluationShaderAsset>(entity);
                        ImGui::Text("Path");
                        ImGui::Text(tesselationEvaluationShader.path.string().c_str());
                        ImGui::Separator();
                    }
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Textures##1"))
            {
                for(auto entity: textureView) {
                    auto &texture = textureView.get<TextureAsset>(entity);
                    ImGui::Text("Path");
                    ImGui::Text(texture.path.string().c_str());

                    ImGui::Text("Loaded:");
                    ImGui::SameLine();
                    ImGui::Text(texture.loaded ? "Yes" : "No");
                    ImGui::Separator();
                }
                ImGui::TreePop();
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();


        
        if(!windowOpened)
            viewListener->RemoveWindow(moduleIndex);
    }
}