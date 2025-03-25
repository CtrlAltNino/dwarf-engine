#include "pch.h"

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "ModelAssetInspector.h"
#include "UI/DwarfUI.h"

namespace Dwarf
{
  ModelAssetInspector::ModelAssetInspector(
    GraphicsApi                       graphicsApi,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetReimporter> assetReimporter,
    std::shared_ptr<IModelPreview>    modelPreview,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IEditorStats>     editorStats)
    : mGraphicsApi(graphicsApi)
    , mAssetDatabase(std::move(assetDatabase))
    , mAssetReimporter(std::move(assetReimporter))
    , mModelPreview(std::move(modelPreview))
    , mInputManager(std::move(inputManager))
    , mEditorStats(std::move(editorStats))
  {
  }

  void
  ModelAssetInspector::Render(IAssetReference& asset)
  {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->ChannelsSplit(2);
    drawList->ChannelsSetCurrent(1);

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
    ImGui::TextWrapped("Model");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Reimport"))
    {
      mAssetReimporter->QueueReimport(asset.GetPath());
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

    ImGui::TextWrapped("TODO here will be settings for model import");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

    ImGui::Text("Preview:");

    mModelPreview->Resize(
      { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x });
    mModelPreview->RenderModelPreview(asset);

    ImVec2 minRect = ImGui::GetCursorScreenPos();
    ImVec2 maxRect(
      ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
      ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().x);

    static bool isRotating = false;
    if (ImGui::IsMouseHoveringRect(
          { ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y },
          { ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
            ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().x }))
    {
      if (mInputManager->GetMouseButtonDown(Dwarf::MOUSE_BUTTON::LEFT))
      {
        isRotating = true;
      }

      float scrollDistance =
        std::max(0.0F,
                 std::min(1.0F,
                          mModelPreview->GetScrollDistance() -
                            (mInputManager->GetMouseScrollDelta().y *
                             (float)mEditorStats->GetDeltaTime() * 8.0f)));

      mModelPreview->SetScrollDistance(scrollDistance);
    }

    if (isRotating &&
        mInputManager->GetMouseButtonUp(Dwarf::MOUSE_BUTTON::LEFT))
    {
      isRotating = false;
    }

    if (isRotating)
    {
      mModelPreview->UpdateRotation(mInputManager->GetMouseDelta());
    }

    drawList->AddImage(mModelPreview->GetTextureId(),
                       minRect,
                       maxRect,
                       ImVec2(0, 1),
                       ImVec2(1, 0));

    drawList->ChannelsSetCurrent(0);

    float endY = maxRect.y;
    ImGui::EndChild();
    ImGui::GetWindowDrawList()->AddRectFilled(
      ImGui::GetItemRectMin(),
      ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x,
             endY + 2 * COMPONENT_PANEL_PADDING),
      IM_COL32(59, 66, 82, 255),
      5.0F);
    // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 *
    // COMPONENT_PANEL_PADDING);
    drawList->ChannelsMerge();
  }
}