#include "pch.hpp"

#include "FooterView.hpp"
#include "Utilities/BrowserLinkOpener.hpp"

namespace Dwarf
{
#define PROJECT_BUTTON_WINDOW_WIDTH (200)
#define PROJECT_INFORMATION_HEIGHT (30)

  FooterView::FooterView(std::shared_ptr<ILauncherData>   data,
                         std::shared_ptr<ILauncherAssets> launcherAssets)
    : mData(std::move(data))
    , mLauncherAssets(std::move(launcherAssets))
  {
  }

  void
  FooterView::RenderFooter(glm::ivec2 windowSize)
  {
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoResize;
    windowFlags |= ImGuiWindowFlags_NoCollapse;
    windowFlags |= ImGuiWindowFlags_NoTitleBar;
    windowFlags |= ImGuiWindowFlags_NoScrollbar;

    ImGui::SetNextWindowPos(
      ImVec2(0, windowSize.y - PROJECT_INFORMATION_HEIGHT));
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, PROJECT_INFORMATION_HEIGHT));
    ImGui::PushStyleColor(ImGuiCol_WindowBg,
                          ImVec4(59 / 255.0, 66 / 255.0, 82 / 255.0, 1.0));
    ImGui::PushStyleColor(ImGuiCol_Border,
                          ImVec4(59 / 255.0, 66 / 255.0, 82 / 255.0, 0));
    ImGui::PushFont(mLauncherAssets->GetTextFont().get());

    if (!ImGui::Begin("Information", NULL, windowFlags))
    {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      return;
    }

    ImGui::Text("An open source project created by @CtrlAltNino");
    std::string versionText = "Release 0.1";
    float       textWidth =
      ImGui::CalcTextSize(versionText.c_str(), nullptr, false).x;

    ImGui::SameLine(0, 15);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(229, 233, 240, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 0));
    static ImVec2 iconSize = ImVec2(18, 18);
    static float  verticalIconOffset = 2;

    {
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() - verticalIconOffset);
      ImTextureID texID =
        (ImTextureID)mLauncherAssets->GetGithubIcon()->GetTextureID();
      ImGui::Image(texID, iconSize);
      if (ImGui::IsItemHovered())
      {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
      }
      if (ImGui::IsItemClicked())
      {
        BrowserLinkOpener::OpenLink(GITHUB_LINK);
      }

      ImGui::SameLine();

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + verticalIconOffset);
      ImGui::Text("GitHub");
      if (ImGui::IsItemHovered())
      {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
      }
      if (ImGui::IsItemClicked())
      {
        BrowserLinkOpener::OpenLink(GITHUB_LINK);
      }
    }

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                         ImGui::GetContentRegionAvail().x - textWidth);
    ImGui::Text("%s", versionText.c_str());

    ImGui::End();
    ImGui::PopStyleColor(2);
    ImGui::PopFont();
  }
}