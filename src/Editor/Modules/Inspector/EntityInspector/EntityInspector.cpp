#include "EntityInspector.h"

#include "UI/DwarfUI.h"

#include <imgui.h>

#define COMPONENT_PANEL_PADDING (8.0f)
#define ADD_BUTTON_WIDTH (40.0f)
#define UNIFORM_DELETE_BUTTON_WIDTH (80.0f)

namespace Dwarf
{
  EntityInspector::EntityInspector(
    std::shared_ptr<IAssetDatabase> assetDatabase)
    : m_AssetDatabase(assetDatabase)
  {
  }

  void
  EntityInspector::Render(std::vector<Entity>& entities)
  {
    // Render the entities
    RenderComponents(entities.at(0));
  }

  void
  EntityInspector::BeginComponent(const char* componentHeader) const
  {
    ImGui::BeginGroup();
    ImVec2 vec2 = ImGui::CalcTextSize(componentHeader);
    ImGui::SetCursorPos(
      ImVec2(ImGui::GetContentRegionAvail().x / 2.0f - (vec2.x / 2.0f),
             ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING));
    ImGui::TextWrapped("%s", componentHeader);
    auto separatorMin =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x +
               COMPONENT_PANEL_PADDING,
             ImGui::GetWindowPos().y + ImGui::GetCursorPos().y +
               COMPONENT_PANEL_PADDING / 2.0f);
    auto separatorMax =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x -
               COMPONENT_PANEL_PADDING,
             separatorMin.y + 2);
    ImGui::GetWindowDrawList()->AddRectFilled(
      separatorMin, separatorMax, COL_BG_DIM);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + COMPONENT_PANEL_PADDING * 2);
  }

  void
  EntityInspector::EndComponent() const
  {
    ImGui::EndGroup();
    ImGui::GetWindowDrawList()->AddRectFilled(
      ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y),
      ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x,
             ImGui::GetItemRectMax().y + 2 * COMPONENT_PANEL_PADDING),
      IM_COL32(59, 66, 82, 255),
      5.0f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 * COMPONENT_PANEL_PADDING);
  }

  template<>
  void
  EntityInspector::RenderComponent<IDComponent>(IDComponent& component)
  {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
    ImGui::TextWrapped("%s", component.GetID().ToString().c_str());
  }

  template<>
  void
  EntityInspector::RenderComponent<NameComponent>(NameComponent& component)
  {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
    ImGui::TextWrapped("Name");
    ImGui::SameLine();
    char* str0 = { (char*)component.Name.c_str() };
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);
    ImGui::InputText("##name_input", str0, sizeof(char) * 64);
    component.Name = std::string(str0);
    ImGui::PopItemWidth();
  }

  template<>
  void
  EntityInspector::RenderComponent<TransformComponent>(
    TransformComponent& component)
  {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    ImGui::TextWrapped("Position");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);
    ImGui::DragFloat3(
      "##transform_position", &component.GetPosition().x, 0.015f);
    ImGui::PopItemWidth();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    ImGui::TextWrapped("Rotation");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    glm::vec3 rot = component.GetEulerAngles();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);
    ImGui::DragFloat3("##transform_rotation", &rot.x, 0.05f);
    component.GetEulerAngles() = rot;
    ImGui::PopItemWidth();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    ImGui::TextWrapped("Scale");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);
    ImGui::DragFloat3("##transform_scale", &component.GetScale().x, 0.015f);
    ImGui::PopItemWidth();
  }

  template<>
  void
  EntityInspector::RenderComponent<LightComponent>(LightComponent& component)
  {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    static int item_current = 0;
    ImGui::TextWrapped("Light type");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);
    ImGui::Combo("##light_type",
                 &item_current,
                 LightComponent::LIGHT_TYPE_NAMES.data(),
                 LightComponent::LIGHT_TYPE_NAMES.size());
    ImGui::PopItemWidth();
    component.GetType() = (LightComponent::LIGHT_TYPE)item_current;

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    ImGui::TextWrapped("Color");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);
    ImGui::ColorEdit3(
      "##light_color", &component.GetColor().r, ImGuiColorEditFlags_None);
    ImGui::PopItemWidth();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);

    ImGui::TextWrapped("Attenuation");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);
    ImGui::DragFloat(
      "##light_attenuation", &component.GetAttenuation(), 0.015f);
    ImGui::PopItemWidth();

    if (component.GetType() == LightComponent::LIGHT_TYPE::POINT_LIGHT)
    {
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
      ImGui::TextWrapped("Radius");
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           COMPONENT_PANEL_PADDING);
      ImGui::DragFloat("##light_point_radius", &component.GetRadius(), 0.015f);
      ImGui::PopItemWidth();
    }

    if (component.GetType() == LightComponent::LIGHT_TYPE::SPOT_LIGHT)
    {
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
      ImGui::TextWrapped("Opening Angle");
      ImGui::SameLine();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                           COMPONENT_PANEL_PADDING);
      ImGui::SliderFloat(
        "##light_spot_angle", &component.GetOpeningAngle(), 0.0f, 180.0f);
      ImGui::PopItemWidth();
    }
  }

  template<>
  void
  EntityInspector::RenderComponent<MeshRendererComponent>(
    MeshRendererComponent& component)
  {
    // TODO: Slot for a mesh asset
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
    ImGui::TextWrapped("Model Asset");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                         COMPONENT_PANEL_PADDING);
    DwarfUI::AssetInput<ModelAsset>(
      m_AssetDatabase, component.GetModelAsset(), "##modelAsset");
    ImGui::PopItemWidth();

    if (component.GetModelAsset().has_value())
    {
      int                 numMaterials = 0;
      std::vector<IMesh>& meshes =
        component.GetModelAsset().value().GetAsset().Meshes();

      for (int i = 0; i < meshes.size(); i++)
      {
        if (meshes[i].GetMaterialIndex() > numMaterials)
        {
          numMaterials = meshes[i].GetMaterialIndex();
        }
      }
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
      ImGui::TextWrapped("Materials");

      if (component.MaterialAssets().size() != numMaterials)
      {
        component.MaterialAssets().resize(numMaterials);
      }

      ImGui::Indent(16.0f);
      for (int i = 0; i < numMaterials; i++)
      {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + COMPONENT_PANEL_PADDING);
        ImGui::TextWrapped("%s", std::to_string(i).c_str());
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x -
                             COMPONENT_PANEL_PADDING);
        DwarfUI::AssetInput<MaterialAsset>(
          m_AssetDatabase,
          component.MaterialAssets().at(i),
          std::format("##materialAsset{}", std::to_string(i)).c_str());
        ImGui::PopItemWidth();
      }
    }
  }

  void
  EntityInspector::RenderComponents(Entity entity)
  {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->ChannelsSplit(2);

    if (entity.HasComponent<NameComponent>())
    {
      draw_list->ChannelsSetCurrent(1);
      BeginComponent("Name");
      RenderComponent(entity.GetComponent<NameComponent>());
      draw_list->ChannelsSetCurrent(0);
      EndComponent();
    }

    if (entity.HasComponent<TransformComponent>())
    {
      draw_list->ChannelsSetCurrent(1);
      BeginComponent("Transform");
      RenderComponent(entity.GetComponent<TransformComponent>());
      draw_list->ChannelsSetCurrent(0);
      EndComponent();
    }

    if (entity.HasComponent<LightComponent>())
    {
      draw_list->ChannelsSetCurrent(1);
      BeginComponent("Light");
      RenderComponent(entity.GetComponent<LightComponent>());
      draw_list->ChannelsSetCurrent(0);
      EndComponent();
    }

    if (entity.HasComponent<MeshRendererComponent>())
    {
      draw_list->ChannelsSetCurrent(1);
      BeginComponent("Mesh Renderer");
      RenderComponent(entity.GetComponent<MeshRendererComponent>());
      draw_list->ChannelsSetCurrent(0);
      EndComponent();
    }
    draw_list->ChannelsMerge();
  }
}