#include "TextureAssetInspector.h"

#include "UI/DwarfUI.h"

namespace Dwarf
{
  TextureAssetInspector::TextureAssetInspector(
    GraphicsApi                       graphicsApi,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetReimporter> assetReimporter,
    std::shared_ptr<IMaterialPreview> materialPreview,
    std::shared_ptr<IInputManager>    inputManager)
    : m_GraphicsApi(graphicsApi)
    , m_AssetDatabase(assetDatabase)
    , m_AssetReimporter(assetReimporter)
    , m_InputManager(inputManager)
  {
  }

  void
  TextureAssetInspector::Render(IAssetReference& asset)
  {
    TextureAsset& textureAsset = dynamic_cast<TextureAsset&>(asset.GetAsset());
    ImDrawList*   draw_list = ImGui::GetWindowDrawList();
    draw_list->ChannelsSplit(2);
    draw_list->ChannelsSetCurrent(1);

    ImGui::BeginChild("##inspector_child",
                      ImGui::GetContentRegionAvail(),
                      false,
                      ImGuiWindowFlags_AlwaysUseWindowPadding);

    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Texture");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Reimport"))
    {
      // m_AssetDatabase->Reimport(asset->GetPath());
      m_AssetReimporter->QueueReimport(asset.GetPath());
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

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
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

    float width = ImGui::GetContentRegionAvail().x;
    ImGui::TextWrapped("Preview:");

    auto texID = (ImTextureID)(textureAsset.GetTexture().GetTextureID());
    // std::cout << "Texture ID: " << texID << std::endl;
    ImGui::Image(texID, ImVec2(width, width));

    draw_list->ChannelsSetCurrent(0);

    float endY = ImGui::GetItemRectMax().y;
    ImGui::EndChild();
    ImGui::GetWindowDrawList()->AddRectFilled(
      ImGui::GetItemRectMin(),
      ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x,
             endY + 2 * COMPONENT_PANEL_PADDING),
      IM_COL32(59, 66, 82, 255),
      5.0f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 * COMPONENT_PANEL_PADDING);
    draw_list->ChannelsMerge();
  }
}