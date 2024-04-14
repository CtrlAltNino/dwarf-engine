#include "dpch.h"

#include <imgui_internal.h>

#include "Editor/Modules/Debug Information/DebugWindow.h"
#include "Core/Asset/AssetDatabase.h"
#include "Core/Asset/AssetComponents.h"
#include "Core/GenericComponents.h"

namespace Dwarf {

  DebugWindow::DebugWindow(Ref<EditorModel> model, int id)
    : GuiModule(model, "Debug", MODULE_TYPE::DEBUG, id)
  {
  }

  void DebugWindow::OnUpdate(double deltaTime)
  {
    // Add Update code that needs to be run before rendering
  }

  void DebugWindow::OnImGuiRender()
  {
    ImGuiWindowFlags window_flags = 0;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(500, 500));
    if (!ImGui::Begin(GetIdentifier().c_str(), &m_WindowOpened, window_flags)) {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      ImGui::PopStyleVar();
      return;
    }

    if (ImGui::CollapsingHeader("Asset Database")) {
      ImGui::Text("Listing all imported assets and their UID's");

      auto materialView =
        AssetDatabase::s_Registry
          ->view<MaterialAsset, PathComponent, NameComponent, IDComponent>();
      auto modelView =
        AssetDatabase::s_Registry
          ->view<ModelAsset, PathComponent, NameComponent, IDComponent>();
      auto sceneView =
        AssetDatabase::s_Registry
          ->view<SceneAsset, PathComponent, NameComponent, IDComponent>();
      auto vertexShaderView = AssetDatabase::s_Registry->view<VertexShaderAsset,
                                                              PathComponent,
                                                              NameComponent,
                                                              IDComponent>();
      auto fragmentShaderView =
        AssetDatabase::s_Registry->view<FragmentShaderAsset,
                                        PathComponent,
                                        NameComponent,
                                        IDComponent>();
      auto geometryShaderView =
        AssetDatabase::s_Registry->view<GeometryShaderAsset,
                                        PathComponent,
                                        NameComponent,
                                        IDComponent>();
      auto computeShaderView =
        AssetDatabase::s_Registry->view<ComputeShaderAsset,
                                        PathComponent,
                                        NameComponent,
                                        IDComponent>();
      auto tesselationControlShaderView =
        AssetDatabase::s_Registry->view<TesselationControlShaderAsset,
                                        PathComponent,
                                        NameComponent,
                                        IDComponent>();
      auto tesselationEvaluationShaderView =
        AssetDatabase::s_Registry->view<TesselationEvaluationShaderAsset,
                                        PathComponent,
                                        NameComponent,
                                        IDComponent>();
      auto textureView =
        AssetDatabase::s_Registry
          ->view<TextureAsset, PathComponent, NameComponent, IDComponent>();

      if (ImGui::TreeNode("Materials##1")) {
        for (auto entity : materialView) {
          auto [path, name, id] =
            materialView.get<PathComponent, NameComponent, IDComponent>(entity);
          ImGui::Text("Name");
          ImGui::Text("%s", name.Name.c_str());
          ImGui::Text("Path");
          ImGui::Text("%s", path.Path.string().c_str());
          ImGui::Text("ID");
          ImGui::Text("%s", std::to_string((uint64_t) * (id.ID)).c_str());
          ImGui::Separator();
          ImGui::Separator();
        }
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Models##1")) {
        for (auto entity : modelView) {
          auto [path, name, id] =
            modelView.get<PathComponent, NameComponent, IDComponent>(entity);
          ImGui::Text("Name");
          ImGui::Text("%s", name.Name.c_str());
          ImGui::Text("Path");
          ImGui::Text("%s", path.Path.string().c_str());
          ImGui::Text("ID");
          ImGui::Text("%s", std::to_string((uint64_t) * (id.ID)).c_str());
          ImGui::Separator();
        }
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Scenes##1")) {
        for (auto entity : sceneView) {
          auto [path, name, id] =
            sceneView.get<PathComponent, NameComponent, IDComponent>(entity);
          ImGui::Text("Name");
          ImGui::Text("%s", name.Name.c_str());
          ImGui::Text("Path");
          ImGui::Text("%s", path.Path.string().c_str());
          ImGui::Text("ID");
          ImGui::Text("%s", std::to_string((uint64_t) * (id.ID)).c_str());
          ImGui::Separator();
        }
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Shaders##1")) {
        if (ImGui::TreeNode("Vertex Shaders##1")) {
          for (auto entity : vertexShaderView) {
            auto [path, name, id] =
              vertexShaderView.get<PathComponent, NameComponent, IDComponent>(
                entity);
            ImGui::Text("Name");
            ImGui::Text("%s", name.Name.c_str());
            ImGui::Text("Path");
            ImGui::Text("%s", path.Path.string().c_str());
            ImGui::Text("ID");
            ImGui::Text("%s", std::to_string((uint64_t) * (id.ID)).c_str());
            ImGui::Separator();
          }
          ImGui::TreePop();
        }

        if (ImGui::TreeNode("Fragment Shaders##1")) {
          for (auto entity : fragmentShaderView) {
            auto [path, name, id] =
              fragmentShaderView.get<PathComponent, NameComponent, IDComponent>(
                entity);
            ImGui::Text("Name");
            ImGui::Text("%s", name.Name.c_str());
            ImGui::Text("Path");
            ImGui::Text("%s", path.Path.string().c_str());
            ImGui::Text("ID");
            ImGui::Text("%s", std::to_string((uint64_t) * (id.ID)).c_str());
            ImGui::Separator();
          }
          ImGui::TreePop();
        }

        if (ImGui::TreeNode("Geometry Shaders##1")) {
          for (auto entity : geometryShaderView) {
            auto [path, name, id] =
              geometryShaderView.get<PathComponent, NameComponent, IDComponent>(
                entity);
            ImGui::Text("Name");
            ImGui::Text("%s", name.Name.c_str());
            ImGui::Text("Path");
            ImGui::Text("%s", path.Path.string().c_str());
            ImGui::Text("ID");
            ImGui::Text("%s", std::to_string((uint64_t) * (id.ID)).c_str());
            ImGui::Separator();
          }
          ImGui::TreePop();
        }

        if (ImGui::TreeNode("Compute Shaders##1")) {
          for (auto entity : computeShaderView) {
            auto [path, name, id] =
              computeShaderView.get<PathComponent, NameComponent, IDComponent>(
                entity);
            ImGui::Text("Name");
            ImGui::Text("%s", name.Name.c_str());
            ImGui::Text("Path");
            ImGui::Text("%s", path.Path.string().c_str());
            ImGui::Text("ID");
            ImGui::Text("%s", std::to_string((uint64_t) * (id.ID)).c_str());
            ImGui::Separator();
          }
          ImGui::TreePop();
        }

        if (ImGui::TreeNode("Tesselation Control Shaders##1")) {
          for (auto entity : tesselationControlShaderView) {
            auto [path, name, id] =
              tesselationControlShaderView
                .get<PathComponent, NameComponent, IDComponent>(entity);
            ImGui::Text("Name");
            ImGui::Text("%s", name.Name.c_str());
            ImGui::Text("Path");
            ImGui::Text("%s", path.Path.string().c_str());
            ImGui::Text("ID");
            ImGui::Text("%s", std::to_string((uint64_t) * (id.ID)).c_str());
            ImGui::Separator();
          }
          ImGui::TreePop();
        }

        if (ImGui::TreeNode("Tesselation Evaluation Shaders##1")) {
          for (auto entity : tesselationEvaluationShaderView) {
            auto [path, name, id] =
              tesselationEvaluationShaderView
                .get<PathComponent, NameComponent, IDComponent>(entity);
            ImGui::Text("Name");
            ImGui::Text("%s", name.Name.c_str());
            ImGui::Text("Path");
            ImGui::Text("%s", path.Path.string().c_str());
            ImGui::Text("ID");
            ImGui::Text("%s", std::to_string((uint64_t) * (id.ID)).c_str());
            ImGui::Separator();
          }
          ImGui::TreePop();
        }
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Textures##1")) {
        for (auto entity : textureView) {
          auto [path, name, id] =
            textureView.get<PathComponent, NameComponent, IDComponent>(entity);
          ImGui::Text("Name");
          ImGui::Text("%s", name.Name.c_str());
          ImGui::Text("Path");
          ImGui::Text("%s", path.Path.string().c_str());
          ImGui::Text("ID");
          ImGui::Text("%s", std::to_string((uint64_t) * (id.ID)).c_str());
          ImGui::Separator();
        }
        ImGui::TreePop();
      }
    }

    ImGui::End();
    ImGui::PopStyleVar();
  }

  void DebugWindow::Deserialize(nlohmann::json moduleData)
  {
    // Add Deserialization for data
  }

  std::string DebugWindow::Serialize()
  {
    return "";
  }
}