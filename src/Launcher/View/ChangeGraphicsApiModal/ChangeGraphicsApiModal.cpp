#include "ChangeGraphicsApiModal.h"
#include "Core/Base.h"
#include <imgui.h>
#include <magic_enum/magic_enum.hpp>
#include <utility>


namespace Dwarf
{
#define PROJECT_BUTTON_WINDOW_WIDTH (200)
#define PROJECT_INFORMATION_HEIGHT (30)

  ChangeGraphicsApiModal::ChangeGraphicsApiModal(
    std::shared_ptr<ILauncherData>   data,
    std::shared_ptr<ISavedProjects>  savedProjects,
    std::shared_ptr<ILauncherAssets> launcherAssets)
    : mData(std::move(data))
    , mSavedProjects(std::move(savedProjects))
    , mLauncherAssets(std::move(launcherAssets))
  {
  }

  void
  ChangeGraphicsApiModal::RenderChangeGraphicsApiModal()
  {
    // Centering Modal
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGuiStyle* style = &ImGui::GetStyle();
    (void)style;
    style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.7);

    // Setting the font for the modal window title
    ImGui::PushFont(mLauncherAssets->GetHeaderFont().get());
    ImGui::SetNextWindowSize(ImVec2(425, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(46, 52, 64, 255));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(94, 129, 172, 255));

    // ==================== Popup Modal ====================
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));

    // ==================== Popup Modal ====================
    if (ImGui::BeginPopupModal("Change Graphics API",
                               NULL,
                               ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoMove))
    {
      ImGui::PushFont(mLauncherAssets->GetTextFont().get());
      // ==================== Information Text ====================
      ImGui::Text(
        "You are about to change the graphics API of a project.\nThis can break the project.\n\
				\n\nIf you project becomes unable to launch, you can safely revert back\nto the previous graphics API.\
				\n\nPlease make sure that:\
				\n- Custom shaders have an equivelant to the selected graphics API\
				\n- Used plugins are compatible with the graphics API you choose\
				\n- Added source code supports the graphics API");

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

      ImGui::Separator();

      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

      static int currentApiIndex =
        (int)mData->GetSelectedProject()->GraphicsApi;
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

        auto apiName = magic_enum::enum_name((GraphicsApi)currentApiIndex);

        if (ImGui::BeginCombo("##graphicsApi", apiName.data()))
        {
          ImDrawList* draw_list = ImGui::GetWindowDrawList();

          // Looping through all the combo entries
          for (int n = 0; n < magic_enum::enum_count<GraphicsApi>(); n++)
          {
            if (apiAvailability[n])
            {
              const bool is_selected = (currentApiIndex == n);

              // Selectable settings
              ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign,
                                  ImVec2(0.5f, 0.5f));
              ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
              ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));
              ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));

              // For drawing a custom Selectable background, we split the
              // channel Now we can draw the text in the foreground, and the
              // colored, rounded rectangle in the background
              draw_list->ChannelsSplit(2);
              draw_list->ChannelsSetCurrent(1);

              if (n > 0)
              {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
              }

              auto apiName =
                magic_enum::enum_name(magic_enum::enum_value<GraphicsApi>(n));
              // ==================== Graphics Selectable ====================
              if (ImGui::Selectable(
                    apiName.data(), is_selected, 0, ImVec2(0, 16 + 10)))
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

      ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(76, 86, 106, 255));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(86, 95, 114, 255));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                            IM_COL32(129, 161, 193, 255));

      // ==================== Apply Button ====================
      if (ImGui::Button("Apply",
                        ImVec2(ImGui::GetContentRegionAvail().x / 2 - 10, 0)))
      {
        mSavedProjects->ChangeGraphicsApi(
          mData->GetSelectedProject().value().Path,
          (GraphicsApi)currentApiIndex);
        ImGui::CloseCurrentPopup();
        mData->SetSelectedProject(std::nullopt);
      }

      if (ImGui::IsItemHovered())
      {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
      }

      // ==================== Cancel Button ====================
      ImGui::SetItemDefaultFocus();
      ImGui::SameLine();
      if (ImGui::Button("Cancel", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
      {
        ImGui::CloseCurrentPopup();
        mData->SetSelectedProject(std::nullopt);
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