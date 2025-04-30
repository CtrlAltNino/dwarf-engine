#include "pch.hpp"

#include "Core/Asset/Database/AssetComponents.hpp"
#include "Core/Scene/Components/LightComponentHandle.hpp"
#include "Core/Scene/Components/MeshRendererComponentHandle.hpp"
#include "Core/Scene/Components/NameComponentHandle.hpp"
#include "Core/Scene/Components/TransformComponentHandle.hpp"
#include "EntityInspector.hpp"
#include "UI/DwarfUI.hpp"
#include <glm/gtc/type_ptr.hpp>

#define COMPONENT_PANEL_PADDING (8.0f)
#define ADD_BUTTON_WIDTH (40.0f)
#define UNIFORM_DELETE_BUTTON_WIDTH (80.0f)

namespace Dwarf
{
  EntityInspector::EntityInspector(
    std::shared_ptr<IDwarfLogger>     logger,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<ILoadedScene>     loadedScene,
    std::shared_ptr<IEditorSelection> editorSelection)
    : mLogger(std::move(logger))
    , mAssetDatabase(std::move(assetDatabase))
    , mLoadedScene(std::move(loadedScene))
    , mEditorSelection(std::move(editorSelection))
  {
  }

  void
  EntityInspector::Render()
  {
    if (mEditorSelection->GetSelectedEntities().empty())
    {
      return;
    }

    // Render the entities
    if (mLoadedScene->GetScene().GetRegistry().valid(
          mEditorSelection->GetSelectedEntities().at(0)))
    {
      RenderComponents(mEditorSelection->GetSelectedEntities().at(0));
    }
    else
    {
      mEditorSelection->ClearEntitySelection();
    }
  }

  void
  EntityInspector::BeginComponent(const char* componentHeader)
  {
    ImGui::BeginGroup();
    ImVec2 vec2 = ImGui::CalcTextSize(componentHeader);
    ImGui::SetCursorPos(
      ImVec2((ImGui::GetContentRegionAvail().x / 2.0F) - (vec2.x / 2.0F),
             ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING));
    ImGui::TextWrapped("%s", componentHeader);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() +
                         (COMPONENT_PANEL_PADDING * 2));
  }

  void
  EntityInspector::EndComponent()
  {
    ImGui::EndGroup();
    ImGui::GetWindowDrawList()->AddRectFilled(
      ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y),
      ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x,
             ImGui::GetItemRectMax().y + (2 * COMPONENT_PANEL_PADDING)),
      IM_COL32(59, 66, 82, 255),
      5.0F);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() +
                         (3 * COMPONENT_PANEL_PADDING));
  }

  template<>
  void
  EntityInspector::RenderComponent<IDComponent>(IDComponent component)
  {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
    ImGui::TextWrapped("%s", component.getId().toString().c_str());
  }

  template<>
  void
  EntityInspector::RenderComponent<NameComponentHandle>(
    NameComponentHandle component)
  {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
    ImGui::TextWrapped("Name");
    ImGui::SameLine();
    // TODO: Needs to be fixed
    char* str0 = { (char*)component.GetName().c_str() };
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);
    if (ImGui::InputText("##name_input", str0, sizeof(char) * 64))
    {
      component.SetName(std::string(str0));
    }
    ImGui::PopItemWidth();
  }

  template<>
  void
  EntityInspector::RenderComponent<TransformComponentHandle>(
    TransformComponentHandle component)
  {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    ImGui::TextWrapped("Position");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);
    glm::vec3 position = component.GetPosition();
    if (ImGui::DragFloat3("##transform_position", &position.x, 0.015F))
    {
      component.SetPosition(position);
    }
    ImGui::PopItemWidth();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    ImGui::TextWrapped("Rotation");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    glm::vec3 rotation = component.GetEulerAngles();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);
    if (ImGui::DragFloat3("##transform_rotation", &rotation.x, 0.05F))
    {
      component.SetEulerAngles(rotation);
    }
    ImGui::PopItemWidth();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    ImGui::TextWrapped("Scale");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);
    glm::vec3 scale = component.GetScale();
    if (ImGui::DragFloat3("##transform_scale", &scale.x, 0.015F))
    {
      component.SetScale(scale);
    }
    ImGui::PopItemWidth();
  }

  template<>
  void
  EntityInspector::RenderComponent<LightComponentHandle>(
    LightComponentHandle component)
  {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    static int itemCurrent = 0;
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);
    LightType lightType = component.GetType();
    if (DwarfUI::ComboEnum("Light type##light_type", lightType))
    {
      component.SetType(lightType);
    }
    ImGui::PopItemWidth();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    ImGui::TextWrapped("Color");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);
    glm::vec3 color = component.GetColor();
    if (ImGui::ColorEdit3(
          "##light_color", glm::value_ptr(color), ImGuiColorEditFlags_None))
    {
      component.SetColor(color);
    }
    ImGui::PopItemWidth();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    ImGui::TextWrapped("Attenuation");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);
    float attenuation = component.GetAttenuation();
    if (ImGui::DragFloat("##light_attenuation", &attenuation, 0.015F))
    {
      component.SetAttenuation(attenuation);
    }
    ImGui::PopItemWidth();

    if (component.GetType() == LightType::PointLight)
    {
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
      ImGui::TextWrapped("Radius");
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           COMPONENT_PANEL_PADDING);
      float radius = component.GetRadius();
      if (ImGui::DragFloat("##light_point_radius", &radius, 0.015F))
      {
        component.SetRadius(radius);
      }
      ImGui::PopItemWidth();
    }

    if (component.GetType() == LightType::SpotLight)
    {
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
      ImGui::TextWrapped("Opening Angle");
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           COMPONENT_PANEL_PADDING);
      float openingAngle = component.GetOpeningAngle();
      if (ImGui::SliderFloat("##light_spot_angle", &openingAngle, 0.0F, 180.0F))
      {
        component.SetOpeningAngle(openingAngle);
      }
      ImGui::PopItemWidth();
    }
  }

  template<>
  void
  EntityInspector::RenderComponent<MeshRendererComponentHandle>(
    MeshRendererComponentHandle componentHandle)
  {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    bool isHidden = componentHandle.GetIsHidden();
    if (ImGui::Checkbox("Hidden", &isHidden))
    {
      componentHandle.SetIsHidden(isHidden);
    }
    ImGui::TextWrapped("Model Asset");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);

    bool                wasNull = componentHandle.GetModelAsset() == nullptr;
    std::optional<UUID> modelAssetId =
      componentHandle.GetModelAsset()->GetUID();

    if (DwarfUI::AssetInput<ModelAsset>(
          mAssetDatabase, modelAssetId, "##modelAsset"))
    {
      componentHandle.SetModelAsset(
        modelAssetId.has_value()
          ? mAssetDatabase->Retrieve(modelAssetId.value())
          : nullptr);

      if (componentHandle.GetModelAsset() != nullptr)
      {
        static UUID memory = componentHandle.GetModelAsset()->GetUID();

        if ((componentHandle.GetModelAsset()->GetUID() != memory) || wasNull)
        {
          // wasNull = false;
          memory = componentHandle.GetModelAsset()->GetUID();
          componentHandle.ClearMaterialAssets();
          componentHandle.SetIdMeshBuffer(nullptr);
          // component.IdMesh() = nullptr;
          for (auto& mesh : (dynamic_cast<ModelAsset&>(
                               componentHandle.GetModelAsset()->GetAsset()))
                              .Meshes())
          {
            componentHandle.SetMaterialAsset(mesh->GetMaterialIndex(), nullptr);
          }
        }
      }
    }
    ImGui::PopItemWidth();

    if (componentHandle.GetModelAsset() != nullptr)
    {
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
      ImGui::TextWrapped("Materials");

      ImGui::Indent(16.0F);
      for (const auto& mat : componentHandle.GetMaterialAssets())
      {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
        ImGui::TextWrapped("%s", std::to_string(mat.first).c_str());
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                             COMPONENT_PANEL_PADDING);
        std::optional<UUID> materialAssetId = mat.second->GetUID();
        if (DwarfUI::AssetInput<MaterialAsset>(
              mAssetDatabase,
              materialAssetId,
              std::format("##materialAsset{}", std::to_string(mat.first))
                .c_str()))
        {
          // mLoadedScene->PropagateSceneChange();

          componentHandle.SetMaterialAsset(
            mat.first,
            materialAssetId.has_value()
              ? mAssetDatabase->Retrieve(materialAssetId.value())
              : nullptr);
        }
        ImGui::PopItemWidth();
      }
    }
    else
    {
      wasNull = true;
    }
  }

  void
  EntityInspector::RenderComponents(entt::entity entity)
  {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->ChannelsSplit(2);
    Entity ent(entity, mLoadedScene->GetScene().GetRegistry());

    if (ent.HasComponent<NameComponent>())
    {
      drawList->ChannelsSetCurrent(1);
      BeginComponent("Name");
      RenderComponent(ent.GetComponentHandle<NameComponentHandle>());
      drawList->ChannelsSetCurrent(0);
      EndComponent();
    }

    if (ent.HasComponent<TransformComponent>())
    {
      drawList->ChannelsSetCurrent(1);
      BeginComponent("Transform");
      RenderComponent(ent.GetComponentHandle<TransformComponentHandle>());
      drawList->ChannelsSetCurrent(0);
      EndComponent();
    }

    if (ent.HasComponent<LightComponent>())
    {
      drawList->ChannelsSetCurrent(1);
      BeginComponent("Light");
      RenderComponent(ent.GetComponentHandle<LightComponentHandle>());
      drawList->ChannelsSetCurrent(0);
      EndComponent();
    }

    if (ent.HasComponent<MeshRendererComponent>())
    {
      drawList->ChannelsSetCurrent(1);
      BeginComponent("Mesh Renderer");
      RenderComponent(ent.GetComponentHandle<MeshRendererComponentHandle>());
      drawList->ChannelsSetCurrent(0);
      EndComponent();
    }
    drawList->ChannelsMerge();
  }
}