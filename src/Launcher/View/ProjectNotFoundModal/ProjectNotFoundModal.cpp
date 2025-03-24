#include "pch.h"

#include "Launcher/LauncherAssets/ILauncherAssets.h"
#include "ProjectNotFoundModal.h"

namespace Dwarf
{
#define PROJECT_BUTTON_WINDOW_WIDTH (200)
#define PROJECT_INFORMATION_HEIGHT (30)

  ProjectNotFoundModal::ProjectNotFoundModal(
    std::shared_ptr<ILauncherData>   data,
    std::shared_ptr<ISavedProjects>  savedProjects,
    std::shared_ptr<ILauncherAssets> launcherAssets)
    : mData(std::move(data))
    , mSavedProjects(std::move(savedProjects))
    , mLauncherAssets(std::move(launcherAssets))
  {
  }

  void
  ProjectNotFoundModal::RenderProjectNotFoundModal()
  {
    // Centering Modal
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    // Setting the dimming background color
    ImGuiStyle* style = &ImGui::GetStyle();
    (void)style;
    style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.7);

    // Setting the font for the modal window title
    ImGui::PushFont(mLauncherAssets->GetHeaderFont().get());
    ImGui::SetNextWindowSize(ImVec2(350, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(46, 52, 64, 255));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(94, 129, 172, 255));

    // ==================== Popup Modal ====================
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));

    // ==================== Popup Modal ====================
    if (ImGui::BeginPopupModal("Project not found!",
                               NULL,
                               ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoMove))
    {
      ImGui::PushFont(mLauncherAssets->GetTextFont().get());
      // ==================== Information Text ====================
      float textWidth =
        ImGui::CalcTextSize(
          mData->GetSelectedProject().value().Path.string().c_str(),
          nullptr,
          false)
          .x;

      ImGui::Text("The project you are trying to open could not be found:");

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
      ImGui::SetCursorPosX(
        ImGui::GetCursorPosX() +
        (ImGui::GetContentRegionAvail().x / 2 - textWidth / 2));
      ImGui::Text("%s",
                  mData->GetSelectedProject().value().Path.string().c_str());

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
      ImGui::Text("Do you want to remove it from the list?");

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

      ImGui::Separator();

      ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(191, 97, 106, 255));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            IM_COL32(198, 111, 120, 255));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                            IM_COL32(216, 134, 142, 255));
      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

      // ==================== Remove Button ====================
      if (ImGui::Button("Remove",
                        ImVec2((ImGui::GetContentRegionAvail().x / 2) - 10, 0)))
      {
        mSavedProjects->RemoveProject(mData->GetSelectedProject().value().Path);
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::IsItemHovered())
      {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
      }

      ImGui::PopStyleColor(3);
      ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(76, 86, 106, 255));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(86, 95, 114, 255));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                            IM_COL32(129, 161, 193, 255));

      // ==================== Cancel Button ====================
      ImGui::SetItemDefaultFocus();
      ImGui::SameLine();
      if (ImGui::Button("Cancel", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
      {
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::IsItemHovered())
      {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
      }

      ImGui::PopStyleVar();
      ImGui::PopStyleColor(3);

      ImGui::PopFont();

      ImGui::EndPopup();
    }
    ImGui::PopStyleVar(5);
    ImGui::PopFont();
    ImGui::PopStyleColor(2);
  }
}