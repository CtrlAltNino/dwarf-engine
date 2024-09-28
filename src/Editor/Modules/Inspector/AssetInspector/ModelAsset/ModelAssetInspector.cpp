#include "ModelAssetInspector.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "UI/DwarfUI.h"
#include <iostream>

namespace Dwarf
{
  ModelAssetInspector::ModelAssetInspector(
    GraphicsApi                       graphicsApi,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetReimporter> assetReimporter,
    std::shared_ptr<IModelPreview>    modelPreview,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IEditorStats>     editorStats)
    : m_GraphicsApi(graphicsApi)
    , m_AssetDatabase(std::move(assetDatabase))
    , m_AssetReimporter(std::move(assetReimporter))
    , m_ModelPreview(std::move(modelPreview))
    , m_InputManager(std::move(inputManager))
    , m_EditorStats(std::move(editorStats))
  {
  }

  void
  ModelAssetInspector::Render(IAssetReference& asset)
  {
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
    ImGui::TextWrapped("Model");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Reimport"))
    {
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

    ImGui::TextWrapped("TODO here will be settings for model import");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

    ImGui::Text("Preview:");

    m_ModelPreview->Resize(
      { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x });
    m_ModelPreview->RenderModelPreview(asset);

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
      if (m_InputManager->GetMouseButtonDown(Dwarf::MOUSE_BUTTON::LEFT))
      {
        isRotating = true;
      }

      float scrollDistance =
        std::max(0.0f,
                 std::min(1.0f,
                          m_ModelPreview->GetScrollDistance() -
                            m_InputManager->GetMouseScrollDelta().y *
                              (float)m_EditorStats->GetDeltaTime() * 8.0f));

      m_ModelPreview->SetScrollDistance(scrollDistance);
    }

    if (isRotating &&
        m_InputManager->GetMouseButtonUp(Dwarf::MOUSE_BUTTON::LEFT))
    {
      isRotating = false;
    }

    if (isRotating)
    {
      m_ModelPreview->UpdateRotation(m_InputManager->GetMouseDelta());
    }

    draw_list->AddImage(m_ModelPreview->GetTextureId(), minRect, maxRect);

    draw_list->ChannelsSetCurrent(0);

    float endY = maxRect.y;
    ImGui::EndChild();
    ImGui::GetWindowDrawList()->AddRectFilled(
      ImGui::GetItemRectMin(),
      ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x,
             endY + 2 * COMPONENT_PANEL_PADDING),
      IM_COL32(59, 66, 82, 255),
      5.0f);
    // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 *
    // COMPONENT_PANEL_PADDING);
    draw_list->ChannelsMerge();
  }
}