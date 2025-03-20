#include "FooterView.h"
#include "Utilities/BrowserLinkOpener.h"
#include <imgui.h>

namespace Dwarf
{
#define PROJECT_BUTTON_WINDOW_WIDTH (200)
#define PROJECT_INFORMATION_HEIGHT (30)

  FooterView::FooterView(std::shared_ptr<ILauncherData>   data,
                         std::shared_ptr<ILauncherAssets> launcherAssets)
    : mData(data)
    , mLauncherAssets(launcherAssets)
  {
  }

  void
  FooterView::RenderFooter(glm::ivec2 windowSize)
  {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoScrollbar;

    ImGui::SetNextWindowPos(
      ImVec2(0, windowSize.y - PROJECT_INFORMATION_HEIGHT));
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, PROJECT_INFORMATION_HEIGHT));
    ImGui::PushStyleColor(ImGuiCol_WindowBg,
                          ImVec4(59 / 255.0, 66 / 255.0, 82 / 255.0, 1.0));
    ImGui::PushStyleColor(ImGuiCol_Border,
                          ImVec4(59 / 255.0, 66 / 255.0, 82 / 255.0, 0));
    ImGui::PushFont(mLauncherAssets->GetTextFont().get());

    if (!ImGui::Begin("Information", NULL, window_flags))
    {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      return;
    }

    ImGui::Text("An open source project created by CtrlAltNino");
    std::string versionText = "early development version";
    float       textWidth =
      ImGui::CalcTextSize(versionText.c_str(), nullptr, false).x;

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
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

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

    {
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() - verticalIconOffset);
      ImTextureID texID =
        (ImTextureID)mLauncherAssets->GetPatreonIcon()->GetTextureID();
      ImGui::Image(texID, iconSize);
      if (ImGui::IsItemHovered())
      {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
      }
      if (ImGui::IsItemClicked())
      {
        BrowserLinkOpener::OpenLink(PATREON_LINK);
      }

      ImGui::SameLine();

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + verticalIconOffset);
      ImGui::Text("Patreon");
      if (ImGui::IsItemHovered())
      {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
      }
      if (ImGui::IsItemClicked())
      {
        BrowserLinkOpener::OpenLink(PATREON_LINK);
      }
    }

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

    {
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() - verticalIconOffset);
      // reinterpret_cast
      ImTextureID texID =
        (ImTextureID)mLauncherAssets->GetXIcon()->GetTextureID();
      ImGui::Image(texID, iconSize);
      if (ImGui::IsItemHovered())
      {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
      }
      if (ImGui::IsItemClicked())
      {
        BrowserLinkOpener::OpenLink(X_LINK);
      }

      ImGui::SameLine();

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + verticalIconOffset);
      ImGui::Text("X");
      if (ImGui::IsItemHovered())
      {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
      }
      if (ImGui::IsItemClicked())
      {
        BrowserLinkOpener::OpenLink(X_LINK);
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