#include "DebugWindow.h"
// #include "Core/Asset/Database/AssetDatabase.h"
// #include "Core/Asset/Database/AssetComponents.h"
#include "Core/GenericComponents.h"
#include <entt/entity/fwd.hpp>
#include <memory>

namespace Dwarf
{

  DebugWindow::DebugWindow(int                             id,
                           std::shared_ptr<IAssetDatabase> assetDatabase)
    : IGuiModule("Debug", MODULE_TYPE::DEBUG, id)
    , m_AssetDatabase(assetDatabase)
  {
  }

  void
  DebugWindow::OnUpdate(double deltaTime)
  {
    // Add Update code that needs to be run before rendering
  }

  void
  DebugWindow::OnImGuiRender()
  {
    ImGuiWindowFlags window_flags = 0;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(500, 500));
    if (!ImGui::Begin(GetIdentifier().c_str(), &m_WindowOpened, window_flags))
    {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      ImGui::PopStyleVar();
      return;
    }

    if (ImGui::CollapsingHeader("Asset Database"))
    {
      ImGui::Text("Listing all imported assets and their UID's");
      std::shared_ptr<entt::registry> registry = m_AssetDatabase->GetRegistry();

      auto materialView =
        registry
          ->view<MaterialAsset, PathComponent, NameComponent, IDComponent>();
      auto modelView =
        registry->view<ModelAsset, PathComponent, NameComponent, IDComponent>();
      auto sceneView =
        registry->view<SceneAsset, PathComponent, NameComponent, IDComponent>();
      auto vertexShaderView = registry->view<VertexShaderAsset,
                                             PathComponent,
                                             NameComponent,
                                             IDComponent>();
      auto fragmentShaderView = registry->view<FragmentShaderAsset,
                                               PathComponent,
                                               NameComponent,
                                               IDComponent>();
      auto geometryShaderView = registry->view<GeometryShaderAsset,
                                               PathComponent,
                                               NameComponent,
                                               IDComponent>();
      auto computeShaderView = registry->view<ComputeShaderAsset,
                                              PathComponent,
                                              NameComponent,
                                              IDComponent>();
      auto tesselationControlShaderView =
        registry->view<TessellationControlShaderAsset,
                       PathComponent,
                       NameComponent,
                       IDComponent>();
      auto tesselationEvaluationShaderView =
        registry->view<TessellationEvaluationShaderAsset,
                       PathComponent,
                       NameComponent,
                       IDComponent>();
      auto textureView =
        registry
          ->view<TextureAsset, PathComponent, NameComponent, IDComponent>();

      if (ImGui::TreeNode("Materials##1"))
      {
        for (auto entity : materialView)
        {
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

      if (ImGui::TreeNode("Models##1"))
      {
        for (auto entity : modelView)
        {
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

      if (ImGui::TreeNode("Scenes##1"))
      {
        for (auto entity : sceneView)
        {
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

      if (ImGui::TreeNode("Shaders##1"))
      {
        if (ImGui::TreeNode("Vertex Shaders##1"))
        {
          for (auto entity : vertexShaderView)
          {
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

        if (ImGui::TreeNode("Fragment Shaders##1"))
        {
          for (auto entity : fragmentShaderView)
          {
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

        if (ImGui::TreeNode("Geometry Shaders##1"))
        {
          for (auto entity : geometryShaderView)
          {
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

        if (ImGui::TreeNode("Compute Shaders##1"))
        {
          for (auto entity : computeShaderView)
          {
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

        if (ImGui::TreeNode("Tesselation Control Shaders##1"))
        {
          for (auto entity : tesselationControlShaderView)
          {
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

        if (ImGui::TreeNode("Tesselation Evaluation Shaders##1"))
        {
          for (auto entity : tesselationEvaluationShaderView)
          {
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

      if (ImGui::TreeNode("Textures##1"))
      {
        for (auto entity : textureView)
        {
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

  void
  DebugWindow::Deserialize(nlohmann::json moduleData)
  {
    // Add Deserialization for data
  }

  nlohmann::json
  DebugWindow::Serialize() const
  {
    return "";
  }
}