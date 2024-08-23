#include "DebugWindow.h"
// #include "Core/Asset/Database/AssetDatabase.h"
// #include "Core/Asset/Database/AssetComponents.h"
#include "Core/GenericComponents.h"
#include <entt/entity/fwd.hpp>
#include <memory>

namespace Dwarf
{

  DebugWindow::DebugWindow(std::shared_ptr<IAssetDatabase> assetDatabase)
    : IGuiModule(ModuleLabel("Debug"),
                 ModuleType(MODULE_TYPE::DEBUG),
                 ModuleID(std::make_shared<UUID>()))
    , m_AssetDatabase(assetDatabase)
  {
  }

  DebugWindow::DebugWindow(nlohmann::json                  serializedModule,
                           std::shared_ptr<IAssetDatabase> assetDatabase)
    : IGuiModule(ModuleLabel("Debug"),
                 ModuleType(MODULE_TYPE::DEBUG),
                 ModuleID(std::make_shared<UUID>(
                   serializedModule["id"].get<std::string>())))
    , m_AssetDatabase(assetDatabase)
  {
  }

  void
  DebugWindow::OnUpdate()
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
          auto [pathComp, nameComp, idComp] =
            materialView.get<PathComponent, NameComponent, IDComponent>(entity);
          ImGui::Text("Name");
          ImGui::Text("%s", nameComp.Name.c_str());
          ImGui::Text("Path");
          ImGui::Text("%s", pathComp.Path.string().c_str());
          ImGui::Text("ID");
          ImGui::Text("%s", idComp.ID->ToString().c_str());
          ImGui::Separator();
          ImGui::Separator();
        }
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Models##1"))
      {
        for (auto entity : modelView)
        {
          auto [pathComp, nameComp, idComp] =
            modelView.get<PathComponent, NameComponent, IDComponent>(entity);
          ImGui::Text("Name");
          ImGui::Text("%s", nameComp.Name.c_str());
          ImGui::Text("Path");
          ImGui::Text("%s", pathComp.Path.string().c_str());
          ImGui::Text("ID");
          ImGui::Text("%s", idComp.ID->ToString().c_str());
          ImGui::Separator();
        }
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Scenes##1"))
      {
        for (auto entity : sceneView)
        {
          auto [pathComp, nameComp, idComp] =
            sceneView.get<PathComponent, NameComponent, IDComponent>(entity);
          ImGui::Text("Name");
          ImGui::Text("%s", nameComp.Name.c_str());
          ImGui::Text("Path");
          ImGui::Text("%s", pathComp.Path.string().c_str());
          ImGui::Text("ID");
          ImGui::Text("%s", idComp.ID->ToString().c_str());
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
            auto [pathComp, nameComp, idComp] =
              vertexShaderView.get<PathComponent, NameComponent, IDComponent>(
                entity);
            ImGui::Text("Name");
            ImGui::Text("%s", nameComp.Name.c_str());
            ImGui::Text("Path");
            ImGui::Text("%s", pathComp.Path.string().c_str());
            ImGui::Text("ID");
            ImGui::Text("%s", idComp.ID->ToString().c_str());
            ImGui::Separator();
          }
          ImGui::TreePop();
        }

        if (ImGui::TreeNode("Fragment Shaders##1"))
        {
          for (auto entity : fragmentShaderView)
          {
            auto [pathComp, nameComp, idComp] =
              fragmentShaderView.get<PathComponent, NameComponent, IDComponent>(
                entity);
            ImGui::Text("Name");
            ImGui::Text("%s", nameComp.Name.c_str());
            ImGui::Text("Path");
            ImGui::Text("%s", pathComp.Path.string().c_str());
            ImGui::Text("ID");
            ImGui::Text("%s", idComp.ID->ToString().c_str());
            ImGui::Separator();
          }
          ImGui::TreePop();
        }

        if (ImGui::TreeNode("Geometry Shaders##1"))
        {
          for (auto entity : geometryShaderView)
          {
            auto [pathComp, nameComp, idComp] =
              geometryShaderView.get<PathComponent, NameComponent, IDComponent>(
                entity);
            ImGui::Text("Name");
            ImGui::Text("%s", nameComp.Name.c_str());
            ImGui::Text("Path");
            ImGui::Text("%s", pathComp.Path.string().c_str());
            ImGui::Text("ID");
            ImGui::Text("%s", idComp.ID->ToString().c_str());
            ImGui::Separator();
          }
          ImGui::TreePop();
        }

        if (ImGui::TreeNode("Compute Shaders##1"))
        {
          for (auto entity : computeShaderView)
          {
            auto [pathComp, nameComp, idComp] =
              computeShaderView.get<PathComponent, NameComponent, IDComponent>(
                entity);
            ImGui::Text("Name");
            ImGui::Text("%s", nameComp.Name.c_str());
            ImGui::Text("Path");
            ImGui::Text("%s", pathComp.Path.string().c_str());
            ImGui::Text("ID");
            ImGui::Text("%s", idComp.ID->ToString().c_str());
            ImGui::Separator();
          }
          ImGui::TreePop();
        }

        if (ImGui::TreeNode("Tesselation Control Shaders##1"))
        {
          for (auto entity : tesselationControlShaderView)
          {
            auto [pathComp, nameComp, idComp] =
              tesselationControlShaderView
                .get<PathComponent, NameComponent, IDComponent>(entity);
            ImGui::Text("Name");
            ImGui::Text("%s", nameComp.Name.c_str());
            ImGui::Text("Path");
            ImGui::Text("%s", pathComp.Path.string().c_str());
            ImGui::Text("ID");
            ImGui::Text("%s", idComp.ID->ToString().c_str());
            ImGui::Separator();
          }
          ImGui::TreePop();
        }

        if (ImGui::TreeNode("Tesselation Evaluation Shaders##1"))
        {
          for (auto entity : tesselationEvaluationShaderView)
          {
            auto [pathComp, nameComp, idComp] =
              tesselationEvaluationShaderView
                .get<PathComponent, NameComponent, IDComponent>(entity);
            ImGui::Text("Name");
            ImGui::Text("%s", nameComp.Name.c_str());
            ImGui::Text("Path");
            ImGui::Text("%s", pathComp.Path.string().c_str());
            ImGui::Text("ID");
            ImGui::Text("%s", idComp.ID->ToString().c_str());
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
          auto [pathComp, nameComp, idComp] =
            textureView.get<PathComponent, NameComponent, IDComponent>(entity);
          ImGui::Text("Name");
          ImGui::Text("%s", nameComp.Name.c_str());
          ImGui::Text("Path");
          ImGui::Text("%s", pathComp.Path.string().c_str());
          ImGui::Text("ID");
          ImGui::Text("%s", idComp.ID->ToString().c_str());
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