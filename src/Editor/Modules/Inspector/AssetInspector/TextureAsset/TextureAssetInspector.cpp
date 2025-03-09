#include "TextureAssetInspector.h"

#include "UI/DwarfUI.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <functional>
#include <imgui.h>
#include <memory>

namespace Dwarf
{
  TextureAssetInspector::TextureAssetInspector(
    GraphicsApi                       graphicsApi,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetReimporter> assetReimporter,
    std::shared_ptr<IMaterialPreview> materialPreview,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IAssetMetadata>   assetMetadata)
    : m_GraphicsApi(graphicsApi)
    , m_AssetDatabase(assetDatabase)
    , m_AssetReimporter(assetReimporter)
    , m_InputManager(inputManager)
    , m_AssetMetadata(assetMetadata)
  {
  }

  void
  TextureAssetInspector::Render(IAssetReference& asset)
  {
    static std::reference_wrapper<TextureAsset> textureAsset =
      dynamic_cast<TextureAsset&>(asset.GetAsset());

    if (asset.GetPath() != m_CurrentTexturePath)
    {
      m_CurrentTexturePath = asset.GetPath();
      textureAsset = dynamic_cast<TextureAsset&>(asset.GetAsset());
      m_CurrentMetadata = m_AssetMetadata->GetMetadata(m_CurrentTexturePath);
      if (m_CurrentMetadata.contains("ImportSettings"))
      {
        m_CurrentImportSettings =
          TextureImportSettings(m_CurrentMetadata["ImportSettings"]);
      }
      else
      {
        m_CurrentImportSettings = TextureImportSettings();
      }
    }

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
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

    ImGui::SameLine();

    if (ImGui::Button("Save Settings"))
    {
      m_CurrentMetadata["ImportSettings"] = m_CurrentImportSettings.Serialize();
      m_AssetMetadata->SetMetadata(m_CurrentTexturePath, m_CurrentMetadata);
    }

    ImGui::TextWrapped("Import Settings: ");

    {
      static const char* textureTypeItems[] = { "Default", "Normal map" };
      const char*        combo_preview_value =
        textureTypeItems[m_CurrentImportSettings.m_TextureType];

      if (ImGui::BeginCombo("Texture Type", combo_preview_value, 0))
      {
        for (int n = 0; n < IM_ARRAYSIZE(textureTypeItems); n++)
        {
          const bool is_selected = (m_CurrentImportSettings.m_TextureType == n);
          if (ImGui::Selectable(textureTypeItems[n], is_selected))
            m_CurrentImportSettings.m_TextureType = (TextureFileType)n;

          // Set the initial focus when opening the combo (scrolling + keyboard
          // navigation focus)
          if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
    }

    ImGui::Spacing();

    {
      static const char* colorSpaceItems[] = { "Linear", "sRGB" };
      const char*        combo_preview_value =
        colorSpaceItems[m_CurrentImportSettings.m_ColorSpace];

      if (ImGui::BeginCombo("Color Space", combo_preview_value, 0))
      {
        for (int n = 0; n < IM_ARRAYSIZE(colorSpaceItems); n++)
        {
          const bool is_selected = (m_CurrentImportSettings.m_ColorSpace == n);
          if (ImGui::Selectable(colorSpaceItems[n], is_selected))
            m_CurrentImportSettings.m_ColorSpace = (ColorSpace)n;

          // Set the initial focus when opening the combo (scrolling + keyboard
          // navigation focus)
          if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
    }

    ImGui::Spacing();

    ImGui::Checkbox("Gamma Correction",
                    &m_CurrentImportSettings.m_GammaCorrection);

    ImGui::Spacing();

    ImGui::Checkbox("Generate Mip Maps",
                    &m_CurrentImportSettings.m_GenerateMipMaps);

    ImGui::Spacing();

    if (m_CurrentImportSettings.m_TextureType == TextureFileType::NormalMap)
    {
      ImGui::Checkbox("Flip Y Channel", &m_CurrentImportSettings.m_FlipY);

      ImGui::Spacing();
    }

    {
      static const char* wrapModeItems[] = { "Clamp", "Mirror", "Repeat" };
      const char*        combo_preview_value =
        wrapModeItems[m_CurrentImportSettings.m_WrapMode];

      if (ImGui::BeginCombo("Wrap Mode", combo_preview_value, 0))
      {
        for (int n = 0; n < IM_ARRAYSIZE(wrapModeItems); n++)
        {
          const bool is_selected = (m_CurrentImportSettings.m_WrapMode == n);
          if (ImGui::Selectable(wrapModeItems[n], is_selected))
            m_CurrentImportSettings.m_WrapMode = (WrapMode)n;

          // Set the initial focus when opening the combo (scrolling + keyboard
          // navigation focus)
          if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
    }

    ImGui::Spacing();

    {
      static const char* filterModeItems[] = { "Nearest", "Bilinear" };
      const char*        combo_preview_value =
        filterModeItems[m_CurrentImportSettings.m_FilterMode];

      if (ImGui::BeginCombo("Filter Mode", combo_preview_value, 0))
      {
        for (int n = 0; n < IM_ARRAYSIZE(filterModeItems); n++)
        {
          const bool is_selected = (m_CurrentImportSettings.m_FilterMode == n);
          if (ImGui::Selectable(filterModeItems[n], is_selected))
            m_CurrentImportSettings.m_FilterMode = (FilterMode)n;

          // Set the initial focus when opening the combo (scrolling + keyboard
          // navigation focus)
          if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
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

    auto texID = (ImTextureID)(textureAsset.get().GetTexture().GetTextureID());
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