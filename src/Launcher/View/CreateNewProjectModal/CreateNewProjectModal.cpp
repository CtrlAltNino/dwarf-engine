#include "Core/Base.h"
#include "pch.h"

#include "CreateNewProjectModal.h"
#include <imgui.h>
#include <nfd.hpp>

namespace Dwarf
{
#define PROJECT_BUTTON_WINDOW_WIDTH (200)
#define PROJECT_INFORMATION_HEIGHT (30)

  CreateNewProjectModal::CreateNewProjectModal(
    std::shared_ptr<ILauncherData>   data,
    std::shared_ptr<IProjectCreator> projectCreator,
    std::shared_ptr<ILauncherAssets> launcherAssets)
    : mData(std::move(data))
    , mProjectCreator(std::move(projectCreator))
    , mLauncherAssets(std::move(launcherAssets))
  {
  }

  void
  CreateNewProjectModal::RenderCreateNewProjectModal()
  {
    // Centering Modal ====================
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    // Setting the font for the modal window title
    ImGui::PushFont(mLauncherAssets->GetHeaderFont().get());
    ImGui::SetNextWindowSize(ImVec2(450, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(46, 52, 64, 255));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(94, 129, 172, 255));
    ImGuiStyle* style = &ImGui::GetStyle();
    (void)style;
    style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.7);

    // ==================== Popup Modal ====================
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
    if (ImGui::BeginPopupModal("Create new project",
                               NULL,
                               ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoResize))
    {
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 8));

      static std::array<char, 128> newProjectName;
      // ==================== Name Input ====================
      {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
        // Title
        ImGui::PushFont(mLauncherAssets->GetHeaderFont().get());
        ImGui::Text("Name");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                             ImGui::GetContentRegionAvail().x - 200);

        // Text Input
        ImGui::PushFont(mLauncherAssets->GetTextFont().get());
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(59, 66, 82, 255));
        ImGui::InputTextWithHint("##projectName",
                                 "Enter name",
                                 newProjectName.data(),
                                 newProjectName.size());
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor();
        ImGui::PopItemWidth();
        ImGui::PopFont();
      }

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
      ImGui::Separator();
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

      static std::filesystem::path newProjectPath =
        mProjectCreator->GetDefaultProjectPath();
      // ==================== Project Path Selector ====================
      {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        // Title
        ImGui::PushFont(mLauncherAssets->GetHeaderFont().get());
        ImGui::Text("Path");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                             ImGui::GetContentRegionAvail().x - 300);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

        draw_list->ChannelsSplit(2);
        draw_list->ChannelsSetCurrent(1);
        // Path preview
        ImGui::PushFont(mLauncherAssets->GetTextFont().get());
        std::string renderText = newProjectPath.string();

        float pathPreviewWidth = ImGui::GetContentRegionAvail().x - 35;

        draw_list->ChannelsSetCurrent(1);
        float textWidth =
          ImGui::CalcTextSize(renderText.c_str(), nullptr, false).x;
        float columnWidth = ImGui::GetContentRegionAvail().x - 40;
        int   availableCharacters =
          (int)(columnWidth / (textWidth / renderText.length())) - 3;

        if ((textWidth > columnWidth))
        {
          renderText.resize(availableCharacters);
          renderText.resize(availableCharacters + 3, '.');
        }
        ImGui::Text("%s", renderText.c_str());

        draw_list->ChannelsSetCurrent(0);
        ImVec2 p_min =
          ImVec2(ImGui::GetItemRectMin().x - 5, ImGui::GetItemRectMin().y - 5);
        ImVec2 p_max = ImVec2(ImGui::GetItemRectMin().x + pathPreviewWidth,
                              ImGui::GetItemRectMax().y + 5);

        ImGui::GetWindowDrawList()->AddRectFilled(
          p_min, p_max, IM_COL32(59, 66, 82, 255), 3);

        draw_list->ChannelsMerge();

        ImGui::SameLine();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                             ImGui::GetContentRegionAvail().x - 30);

        float width = ImGui::GetContentRegionAvail().x;

        // Browse path button
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        if (ImGui::Button("...", ImVec2(width, 25)))
        {
          // initialize NFD
          NFD::Guard nfdGuard;
          // auto-freeing memory
          NFD::UniquePath outPath;
          // show the dialog
          nfdresult_t result = NFD::PickFolder(outPath);

          if (result == NFD_OKAY)
          {
            newProjectPath = std::string(outPath.get());
          }
          else if (result == NFD_CANCEL)
          {
            // puts("User pressed cancel.");
          }
          else
          {
            // printf("Error: %s\n", NFD_GetError());
          }
        }
        ImGui::PopStyleVar(2);

        if (ImGui::IsItemHovered())
        {
          ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        ImGui::PopFont();
      }

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
      ImGui::Separator();
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

      static int currentTemplateIndex = 0;
      // ==================== Project Template Selector ====================
      {
        // Template Title
        ImGui::PushFont(mLauncherAssets->GetHeaderFont().get());
        ImGui::Text("Template");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                             ImGui::GetContentRegionAvail().x - 150);

        // Setting up combo
        std::vector<std::string> templates = {
          "Blank", "Olaf", "Crytek Sponza", "Sci-fi Helmet"
        };

        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
        ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign,
                            ImVec2(0.5f, 0.5f));
        ImGui::PushFont(mLauncherAssets->GetTextFont().get());

        // Coloring the combo preview
        ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(59, 66, 82, 255));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,
                              IM_COL32(76, 86, 106, 255));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(255, 0, 0, 255));

        // Coloring the selected combo item
        ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(59, 66, 82, 255));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered,
                              IM_COL32(67, 76, 94, 255));

        // Coloring the combo popup background
        ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(46, 52, 64, 255));

        if (ImGui::BeginCombo("##template",
                              templates[currentTemplateIndex].c_str()))
        {
          ImDrawList* draw_list = ImGui::GetWindowDrawList();

          // Looping through all the combo entries
          for (int n = 0; n < templates.size(); n++)
          {
            const bool is_selected = (currentTemplateIndex == n);

            // Selectable settings
            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign,
                                ImVec2(0.5f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));

            // For drawing a custom Selectable background, we split the channel
            // Now we can draw the text in the foreground, and the colored,
            // rounded rectangle in the background
            draw_list->ChannelsSplit(2);
            draw_list->ChannelsSetCurrent(1);
            if (n > 0)
            {
              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
            }

            //  ==================== Rendering Selectable ====================
            if (ImGui::Selectable(
                  templates[n].c_str(), is_selected, 0, ImVec2(0, 16 + 10)))
            {
              currentTemplateIndex = n;
            }

            // Reset Style
            ImGui::PopStyleVar(1);
            ImGui::PopStyleColor(3);

            // Drawing the background rectangle
            if (ImGui::IsItemHovered())
            {
              ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
              draw_list->ChannelsSetCurrent(0);
              ImVec2 p_min = ImGui::GetItemRectMin();
              ImVec2 p_max = ImGui::GetItemRectMax();
              ImU32  rectCol = ImGui::IsMouseDown(ImGuiMouseButton_Left)
                                 ? IM_COL32(76, 86, 106, 255)
                                 : IM_COL32(67, 76, 94, 255);

              ImGui::GetWindowDrawList()->AddRectFilled(
                p_min, p_max, rectCol, 5);
            }
            else if (is_selected)
            {
              draw_list->ChannelsSetCurrent(0);
              ImVec2 p_min = ImGui::GetItemRectMin();
              ImVec2 p_max = ImGui::GetItemRectMax();
              ImU32  rectCol = IM_COL32(59, 66, 82, 255);

              ImGui::GetWindowDrawList()->AddRectFilled(
                p_min, p_max, rectCol, 5);
            }

            draw_list->ChannelsMerge();

            // Set the initial focus when opening the combo (scrolling +
            // keyboard navigation focus)
            if (is_selected) ImGui::SetItemDefaultFocus();
          }

          ImGui::EndCombo();
        }
        ImGui::PopFont();

        // Use Hand cursor when hovering over the combo
        if (ImGui::IsItemHovered())
        {
          ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        // Reset remaining style attributes
        ImGui::PopStyleVar(4);
        ImGui::PopStyleColor(6);
      }

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
      ImGui::Separator();
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

      static int currentApiIndex = 2;
      // ==================== Graphics API Selection Dropdown
      // ====================
      {
        // Graphics Title
        ImGui::PushFont(mLauncherAssets->GetHeaderFont().get());
        ImGui::Text("Graphics API");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                             ImGui::GetContentRegionAvail().x - 150);

        // Setting up combo
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
        ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign,
                            ImVec2(0.5f, 0.5f));
        ImGui::PushFont(mLauncherAssets->GetTextFont().get());

        // Coloring the combo preview
        ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(59, 66, 82, 255));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,
                              IM_COL32(76, 86, 106, 255));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(255, 0, 0, 255));

        // Coloring the selected combo item
        ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(59, 66, 82, 255));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered,
                              IM_COL32(67, 76, 94, 255));

        // Coloring the combo popup background
        ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(46, 52, 64, 255));

        auto apiName = magic_enum::enum_name(
          magic_enum::enum_value<GraphicsApi>(currentApiIndex));

        if (ImGui::BeginCombo("##graphicsApi", apiName.data()))
        {
          ImDrawList* draw_list = ImGui::GetWindowDrawList();

          // Looping through all the combo entries
          for (int n = 1; n < 4; n++)
          {
            if (apiPlatformSupport[n - 1])
            {
              const bool is_selected = (currentApiIndex == n);

              // Selectable settings
              ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign,
                                  ImVec2(0.5F, 0.5F));
              ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
              ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));
              ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));

              // For drawing a custom Selectable background, we split the
              // channel Now we can draw the text in the foreground, and the
              // colored, rounded rectangle in the background
              draw_list->ChannelsSplit(2);
              draw_list->ChannelsSetCurrent(1);

              if (n > 1)
              {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
              }

              auto apiName =
                magic_enum::enum_name(magic_enum::enum_value<GraphicsApi>(n));

              // ==================== Rendering Selectable ====================
              if (ImGui::Selectable(apiName.data(),
                                    is_selected,
                                    apiImplementationState[n - 1]
                                      ? 0
                                      : ImGuiSelectableFlags_Disabled,
                                    ImVec2(0, 16 + 10)))
              {
                currentApiIndex = n;
              }

              // Reset Style
              ImGui::PopStyleVar(1);
              ImGui::PopStyleColor(3);

              // Drawing the background rectangle
              if (ImGui::IsItemHovered())
              {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                draw_list->ChannelsSetCurrent(0);
                ImVec2 pMin = ImGui::GetItemRectMin();
                ImVec2 pMax = ImGui::GetItemRectMax();
                ImU32  rectCol = ImGui::IsMouseDown(ImGuiMouseButton_Left)
                                   ? IM_COL32(76, 86, 106, 255)
                                   : IM_COL32(67, 76, 94, 255);

                ImGui::GetWindowDrawList()->AddRectFilled(
                  pMin, pMax, rectCol, 5);
              }
              else if (is_selected)
              {
                draw_list->ChannelsSetCurrent(0);
                ImVec2 pMin = ImGui::GetItemRectMin();
                ImVec2 pMax = ImGui::GetItemRectMax();
                ImU32  rectCol = IM_COL32(59, 66, 82, 255);

                ImGui::GetWindowDrawList()->AddRectFilled(
                  pMin, pMax, rectCol, 5);
              }

              draw_list->ChannelsMerge();

              // Set the initial focus when opening the combo (scrolling +
              // keyboard navigation focus)
              if (is_selected)
              {
                ImGui::SetItemDefaultFocus();
              }
            }
          }

          ImGui::EndCombo();
        }
        ImGui::PopFont();

        // Use Hand cursor when hovering over the combo
        if (ImGui::IsItemHovered())
        {
          ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        // Reset remaining style attributes
        ImGui::PopStyleVar(4);
        ImGui::PopStyleColor(6);
      }

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
      ImGui::Separator();
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

      ImGui::PushStyleColor(ImGuiCol_Button,
                            ImVec4(76 / 255.0, 86 / 255.0, 106 / 255.0, 1));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(86 / 255.0, 95 / 255.0, 114 / 255.0, 1));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                            ImVec4(129 / 255.0, 161 / 255.0, 193 / 255.0, 1));
      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

      // ==================== Create Button ====================
      {
        ImGui::PushFont(mLauncherAssets->GetTextFont().get());
        if (ImGui::Button(
              "Create", ImVec2((ImGui::GetContentRegionAvail().x / 2) - 8, 40)))
        {
          // Create folder at the path with the project name
          // Create and fill projectSettings.dproj appropriately
          // If using a template, copy the corresponding files
          // Add project to saved project list
          // Launch project after creating, or just close the popup?
          // CreateProject(newProjectName, newProjectPath.c_str(),
          // (GraphicsApi)currentApiIndex,
          // (ProjectTemplate)currentTemplateIndex);
          mProjectCreator->CreateProject(newProjectName.data(),
                                         newProjectPath,
                                         (GraphicsApi)currentApiIndex,
                                         (ProjectTemplate)currentTemplateIndex);
          newProjectName[0] = '\0';
          currentTemplateIndex = 0;
          currentApiIndex = 0;

          ImGui::CloseCurrentPopup();
        }
        ImGui::PopFont();

        if (ImGui::IsItemHovered())
        {
          ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
      }

      // ==================== Cancel Button ====================
      {
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        ImGui::PushFont(mLauncherAssets->GetTextFont().get());
        if (ImGui::Button("Cancel",
                          ImVec2(ImGui::GetContentRegionAvail().x, 40)))
        {
          newProjectName[0] = '\0';
          currentTemplateIndex = 0;
          currentApiIndex = 2;
          ImGui::CloseCurrentPopup();
        }
        ImGui::PopFont();

        if (ImGui::IsItemHovered())
        {
          ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
      }

      ImGui::PopStyleVar();
      ImGui::PopStyleColor(3);

      ImGui::PopStyleVar();
      ImGui::EndPopup();
    }
    ImGui::PopStyleVar(5);
    ImGui::PopFont();
    ImGui::PopStyleColor(2);
  }
}