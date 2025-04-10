#include "pch.h"

#include "SceneAssetInspector.h"
#include "UI/DwarfUI.h"
#include <imgui.h>

namespace Dwarf
{
  SceneAssetInspector::SceneAssetInspector(
    GraphicsApi                       graphicsApi,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetReimporter> assetReimporter,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<ISceneIO>         sceneIO,
    std::shared_ptr<ILoadedScene>     loadedScene,
    std::shared_ptr<IFileHandler>     fileHandler)
    : mGraphicsApi(graphicsApi)
    , mAssetDatabase(std::move(assetDatabase))
    , mAssetReimporter(std::move(assetReimporter))
    , mInputManager(std::move(inputManager))
    , mSceneIO(std::move(sceneIO))
    , mLoadedScene(std::move(loadedScene))
    , mFileHandler(std::move(fileHandler))
  {
  }

  void
  SceneAssetInspector::Render(IAssetReference& asset)
  {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->ChannelsSplit(2);
    drawList->ChannelsSetCurrent(1);

    if (!ImGui::BeginChild("##inspector_child",
                           ImVec2(0, 0),
                           0,
                           ImGuiWindowFlags_AlwaysUseWindowPadding))
    {
      ImGui::EndChild();
      return;
    }

    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0F);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0F);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0F);
    ImGui::TextWrapped("Dwarf Engine Scene");

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Reimport"))
    {
      mAssetReimporter->QueueReimport(asset.GetPath());
    }

    ImGui::SameLine(0, 5);

    if (ImGui::Button("Load scene") &&
        mFileHandler->FileExists(asset.GetPath()))
    {
      mLoadedScene->SetScene(mSceneIO->LoadScene(asset));
    }

    drawList->ChannelsSetCurrent(0);

    ImGui::EndChild();
    ImGui::GetWindowDrawList()->AddRectFilled(
      ImGui::GetItemRectMin(),
      ImVec2(ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvail().x,
             ImGui::GetCursorPosY() + 2 * COMPONENT_PANEL_PADDING),
      IM_COL32(59, 66, 82, 255),
      5.0F);
    drawList->ChannelsMerge();
  }
}