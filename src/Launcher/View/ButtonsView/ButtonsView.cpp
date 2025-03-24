#include "pch.h"

#include "ButtonsView.h"
#include <imgui.h>

namespace Dwarf
{
#define PROJECT_BUTTON_WINDOW_WIDTH (200)
#define PROJECT_INFORMATION_HEIGHT (30)

  ButtonsView::ButtonsView(std::shared_ptr<ILauncherData>   data,
                           std::shared_ptr<ISavedProjects>  savedProjects,
                           std::shared_ptr<ILauncherAssets> launcherAssets)
    : mData(std::move(data))
    , mSavedProjects(std::move(savedProjects))
    , mLauncherAssets(std::move(launcherAssets))
  {
  }

  void
  ButtonsView::RenderButtons(glm::ivec2 windowSize)
  {
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoResize;
    windowFlags |= ImGuiWindowFlags_NoCollapse;
    windowFlags |= ImGuiWindowFlags_NoTitleBar;

    ImGui::SetNextWindowPos(
      ImVec2(windowSize.x - PROJECT_BUTTON_WINDOW_WIDTH, 0));
    ImGui::SetNextWindowSize(ImVec2(PROJECT_BUTTON_WINDOW_WIDTH,
                                    windowSize.y - PROJECT_INFORMATION_HEIGHT));
    ImGui::PushStyleColor(ImGuiCol_WindowBg,
                          ImVec4(67 / 255.0, 76 / 255.0, 94 / 255.0, 1.0));
    ImGui::PushStyleColor(ImGuiCol_Border,
                          ImVec4(59 / 255.0, 66 / 255.0, 82 / 255.0, 0));

    if (!ImGui::Begin("Project Buttons", nullptr, windowFlags))
    {
      ImGui::End();
      return;
    }

    ImGui::PushFont(mLauncherAssets->GetHeaderFont().get());
    ImGui::Text("Add Project");
    ImGui::PopFont();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
    ImGui::Separator();
    ImGui::PushFont(mLauncherAssets->GetTextFont().get());
    ImGui::PushStyleColor(ImGuiCol_Button,
                          ImVec4(76 / 255.0, 86 / 255.0, 106 / 255.0, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4(86 / 255.0, 95 / 255.0, 114 / 255.0, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          ImVec4(129 / 255.0, 161 / 255.0, 193 / 255.0, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    if (ImGui::Button("Create new project",
                      ImVec2(ImGui::GetContentRegionAvail().x, 75)))
    {
      mData->SetState(ProjectChooserState::CreateNewProject);
    }

    if (ImGui::IsItemHovered())
    {
      ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    // Put into context menu
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    if (ImGui::Button("Add existing project",
                      ImVec2(ImGui::GetContentRegionAvail().x, 75)))
    {
      mSavedProjects->OpenAddProjectDialog();
    }

    if (ImGui::IsItemHovered())
    {
      ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);

    ImGui::PushFont(mLauncherAssets->GetHeaderFont().get());
    ImGui::Text("Options");
    ImGui::PopFont();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
    ImGui::Separator();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    if (ImGui::Button("Manage plugins",
                      ImVec2(ImGui::GetContentRegionAvail().x, 75)))
    {
      // Open plugin manager modal
      // Contains list of plugins that are present and their state (working, not
      // working, warnings) Add plugin button Button to open up the plugin
      // folder Activating and deactivating plugins happens on a project level
    }

    if (ImGui::IsItemHovered())
    {
      ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    if (ImGui::Button("Settings", ImVec2(ImGui::GetContentRegionAvail().x, 75)))
    {
      // Open settings modal
      // Manage settings for the launcher and general engine settings?
    }

    if (ImGui::IsItemHovered())
    {
      ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(5);
    ImGui::PopFont();
    ImGui::End();
  }
}