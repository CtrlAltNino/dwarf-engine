#include "pch.hpp"

#include "Core/Asset/Database/AssetComponents.hpp"
#include "TextureAssetInspector.hpp"
#include "UI/DwarfUI.hpp"
#include "Utilities/ImageUtilities/TextureCommon.hpp"
#include <imgui.h>

namespace Dwarf
{
  TextureAssetInspector::TextureAssetInspector(
    GraphicsApi                       graphicsApi,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetReimporter> assetReimporter,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IAssetMetadata>   assetMetadata)
    : mGraphicsApi(graphicsApi)
    , mAssetDatabase(std::move(assetDatabase))
    , mAssetReimporter(std::move(assetReimporter))
    , mInputManager(std::move(inputManager))
    , mAssetMetadata(std::move(assetMetadata))
  {
  }

  void
  TextureAssetInspector::Render(IAssetReference& asset)
  {
    static std::reference_wrapper<TextureAsset> textureAsset =
      dynamic_cast<TextureAsset&>(asset.GetAsset());

    if (asset.GetPath() != mCurrentTexturePath)
    {
      mCurrentTexturePath = asset.GetPath();
      textureAsset = dynamic_cast<TextureAsset&>(asset.GetAsset());
      mCurrentMetadata = mAssetMetadata->GetMetadata(mCurrentTexturePath);
      if (mCurrentMetadata.contains("ImportSettings"))
      {
        mCurrentImportSettings =
          TextureImportSettings(mCurrentMetadata["ImportSettings"]);
      }
      else
      {
        mCurrentImportSettings = TextureImportSettings();
      }
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->ChannelsSplit(2);
    drawList->ChannelsSetCurrent(1);

    if (!ImGui::BeginChild("##inspector_child",
                           ImGui::GetContentRegionAvail(),
                           0,
                           ImGuiWindowFlags_AlwaysUseWindowPadding))
    {
      ImGui::EndChild();
      return;
    }

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
      // mAssetDatabase->Reimport(asset->GetPath());
      mAssetReimporter->QueueReimport(asset.GetPath());
    }

    ImGui::SameLine();

    if (ImGui::Button("Save Settings"))
    {
      mCurrentMetadata["ImportSettings"] = mCurrentImportSettings.Serialize();
      mAssetMetadata->SetMetadata(mCurrentTexturePath, mCurrentMetadata);
    }

    ImGui::TextWrapped("Import Settings: ");

    {
      static const char* textureTypeItems[] = { "Default", "Normal map" };
      const char*        comboPreviewValue =
        textureTypeItems[mCurrentImportSettings.mTextureType];

      if (ImGui::BeginCombo("Texture Type", comboPreviewValue, 0))
      {
        for (int n = 0; n < IM_ARRAYSIZE(textureTypeItems); n++)
        {
          const bool isSelected = (mCurrentImportSettings.mTextureType == n);
          if (ImGui::Selectable(textureTypeItems[n], isSelected))
            mCurrentImportSettings.mTextureType = (TextureFileType)n;

          // Set the initial focus when opening the combo (scrolling +
          // keyboard navigation focus)
          if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
    }

    ImGui::Spacing();

    {
      static const char* colorSpaceItems[] = { "Linear", "sRGB" };
      const char*        comboPreviewValue =
        colorSpaceItems[mCurrentImportSettings.mColorSpace];

      if (ImGui::BeginCombo("Color Space", comboPreviewValue, 0))
      {
        for (int n = 0; n < IM_ARRAYSIZE(colorSpaceItems); n++)
        {
          const bool is_selected = (mCurrentImportSettings.mColorSpace == n);
          if (ImGui::Selectable(colorSpaceItems[n], is_selected))
          {
            mCurrentImportSettings.mColorSpace = (ColorSpace)n;
          }

          // Set the initial focus when opening the combo (scrolling +
          // keyboard navigation focus)
          if (is_selected)
          {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
    }

    ImGui::Spacing();

    ImGui::Checkbox("Generate Mip Maps",
                    &mCurrentImportSettings.mGenerateMipMaps);

    ImGui::Spacing();

    if (mCurrentImportSettings.mTextureType == TextureFileType::NormalMap)
    {
      ImGui::Checkbox("Flip Y Channel", &mCurrentImportSettings.mFlipY);

      ImGui::Spacing();
    }

    {
      static const char* wrapModeItems[] = { "Clamp", "Mirror", "Repeat" };
      const char*        comboPreviewValue =
        wrapModeItems[mCurrentImportSettings.mWrapMode];

      if (ImGui::BeginCombo("Wrap Mode", comboPreviewValue, 0))
      {
        for (int n = 0; n < IM_ARRAYSIZE(wrapModeItems); n++)
        {
          const bool is_selected = (mCurrentImportSettings.mWrapMode == n);
          if (ImGui::Selectable(wrapModeItems[n], is_selected))
          {
            mCurrentImportSettings.mWrapMode = (WrapMode)n;
          }

          // Set the initial focus when opening the combo (scrolling +
          // keyboard navigation focus)
          if (is_selected)
          {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
    }

    ImGui::Spacing();

    {
      static const char* filterModeItems[] = { "Nearest", "Bilinear" };
      const char*        comboPreviewValue =
        filterModeItems[mCurrentImportSettings.mFilterMode];

      if (ImGui::BeginCombo("Filter Mode", comboPreviewValue, 0))
      {
        for (int n = 0; n < IM_ARRAYSIZE(filterModeItems); n++)
        {
          const bool is_selected = (mCurrentImportSettings.mFilterMode == n);
          if (ImGui::Selectable(filterModeItems[n], is_selected))
          {
            mCurrentImportSettings.mFilterMode = (FilterMode)n;
          }

          // Set the initial focus when opening the combo (scrolling +
          // keyboard navigation focus)
          if (is_selected)
          {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

    auto separatorMin =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x +
               COMPONENT_PANEL_PADDING,
             ImGui::GetWindowPos().y + ImGui::GetCursorPos().y +
               (COMPONENT_PANEL_PADDING / 2.0F));
    auto separatorMax =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x -
               COMPONENT_PANEL_PADDING,
             separatorMin.y + 2);
    ImGui::GetWindowDrawList()->AddRectFilled(
      separatorMin, separatorMax, COL_BG_DIM);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

    float width = ImGui::GetContentRegionAvail().x;
    ImGui::TextWrapped("Preview:");

    auto texID = (ImTextureID)(textureAsset.get().GetTexture()->GetTextureID());
    ImGui::Image(texID, ImVec2(width, width));

    drawList->ChannelsSetCurrent(0);

    float endY = ImGui::GetItemRectMax().y;
    ImGui::EndChild();
    ImGui::GetWindowDrawList()->AddRectFilled(
      ImGui::GetItemRectMin(),
      ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x,
             endY + (2 * COMPONENT_PANEL_PADDING)),
      IM_COL32(59, 66, 82, 255),
      5.0F);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() +
                         (3 * COMPONENT_PANEL_PADDING));
    drawList->ChannelsMerge();
  }
}