#include "pch.hpp"

#include "ProjectListView.hpp"
#include "Utilities/TimeUtilities.hpp"

namespace Dwarf
{
#define PROJECT_BUTTON_WINDOW_WIDTH (200)
#define PROJECT_INFORMATION_HEIGHT (30)

  ProjectListView::ProjectListView(
    std::shared_ptr<ILauncherData>        data,
    std::shared_ptr<ISavedProjects>       savedProjects,
    std::shared_ptr<ISavedProjectsSorter> savedProjectsSorter,
    std::shared_ptr<IFileHandler>         fileHandler,
    std::shared_ptr<ILauncherAssets>      launcherAssets)
    : mData(std::move(data))
    , mSavedProjects(std::move(savedProjects))
    , mSavedProjectsSorter(std::move(savedProjectsSorter))
    , mFileHandler(std::move(fileHandler))
    , mLauncherAssets(std::move(launcherAssets))
  {
  }

  void
  ProjectListView::RenderProjectList(glm::ivec2 windowSize)
  {
    const std::vector<SavedProject> projectList =
      mSavedProjects->GetSavedProjects();
    std::vector<std::filesystem::path> projectsToRemove;

    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoResize;
    windowFlags |= ImGuiWindowFlags_NoCollapse;
    windowFlags |= ImGuiWindowFlags_NoTitleBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 8));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(windowSize.x - PROJECT_BUTTON_WINDOW_WIDTH,
                                    windowSize.y - PROJECT_INFORMATION_HEIGHT));
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(46, 52, 64, 255));

    if (!ImGui::Begin("Project List", NULL, windowFlags))
    {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
    ImGui::PushFont(mLauncherAssets->GetHeaderFont().get());
    ImGui::Text("Project List");
    ImGui::PopFont();
    ImGui::Separator();
    ImGui::PopStyleVar(1);

    const int   columnsCount = 4;
    const float rowHeight = 50;
    const float headerRowHeight = 40;
    ImVec2      cellPadding = ImVec2(10.0F, 10.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cellPadding);
    ImGui::PushStyleColor(ImGuiCol_TableBorderLight, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, IM_COL32(59, 66, 82, 255));
    if (ImGui::BeginTable("Project entries",
                          columnsCount,
                          ImGuiTableFlags_Borders |
                            ImGuiTableFlags_SizingFixedFit |
                            ImGuiTableFlags_PreciseWidths))
    {
      float initialWidth = ImGui::GetContentRegionAvail().x - 80;
      ImGui::TableSetupColumn("Name",
                              ImGuiTableColumnFlags_NoResize |
                                ImGuiTableColumnFlags_NoReorder |
                                ImGuiTableColumnFlags_WidthFixed,
                              initialWidth * 0.2F);
      ImGui::TableSetupColumn("Path",
                              ImGuiTableColumnFlags_NoResize |
                                ImGuiTableColumnFlags_NoReorder |
                                ImGuiTableColumnFlags_WidthFixed,
                              initialWidth * 0.4F);
      ImGui::TableSetupColumn("Last opened",
                              ImGuiTableColumnFlags_NoResize |
                                ImGuiTableColumnFlags_NoReorder |
                                ImGuiTableColumnFlags_WidthFixed,
                              initialWidth * 0.25F);
      ImGui::TableSetupColumn("API",
                              ImGuiTableColumnFlags_NoResize |
                                ImGuiTableColumnFlags_NoReorder |
                                ImGuiTableColumnFlags_WidthFixed,
                              initialWidth * 0.15F);
      // ImGui::Columns(4);
      // ImGui::SetColumnWidth(-1, 50);

      // Dummy entire-column selection storage
      // FIXME: It would be nice to actually demonstrate full-featured selection
      // using those checkbox.
      ImDrawList* drawList = ImGui::GetWindowDrawList();

      // Instead of calling TableHeadersRow() we'll submit custom headers
      // ourselves
      ImGui::TableNextRow(ImGuiTableRowFlags_Headers, headerRowHeight);
      ImGui::PushFont(mLauncherAssets->GetHeaderFont().get());
      ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(76, 86, 106, 255));
      ImGui::PushStyleColor(ImGuiCol_HeaderActive,
                            IM_COL32(129, 161, 193, 255));
      for (int column = 0; column < columnsCount; column++)
      {
        ImGui::TableSetColumnIndex(column);
        ImGui::PushID(column);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() +
                             (headerRowHeight / 2 - cellPadding.y / 2) -
                             (ImGui::GetFontSize() / 2));
        ImGui::TableHeader(ImGui::TableGetColumnName(column));

        if (ImGui::IsItemHovered())
        {
          ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        if (ImGui::IsItemClicked())
        {
          mSavedProjectsSorter->UpdateSortOrder(ProjectListColumn(column));
          mSavedProjectsSorter->SortSavedProjects(
            mSavedProjects->GetSavedProjects());
        }

        ImGui::PopID();
      }
      ImGui::PopStyleColor(2);
      ImGui::PopFont();

      ImGui::PushFont(mLauncherAssets->GetTextFont().get());
      for (const auto& project : projectList)
      {
        ImGui::TableNextRow(ImGuiTableRowFlags_None, rowHeight);
        for (int column = 0; (column < columnsCount); column++)
        {
          ImGui::TableSetColumnIndex(column);
          std::string cellText;
          switch (column)
          {
            case 0: cellText = project.ProjectName; break;
            case 1: cellText = project.Path.string(); break;
            case 2:
              {
                time_t lastOpenedTime = project.LastOpened;
                cellText =
                  lastOpenedTime == 0
                    ? "never"
                    : Dwarf::TimeUtilities::CalculateTimePassedSinceNow(
                        project.LastOpened);
                break;
              }
            case 3: cellText = magic_enum::enum_name(project.Api); break;
          }

          if (column == 0)
          {
            float textWidth =
              ImGui::CalcTextSize(cellText.c_str(), (const char*)0, false).x;
            float columnWidth = ImGui::GetContentRegionAvail().x - 8;
            int   availableCharacters =
              (int)(columnWidth / (textWidth / cellText.length()));

            if (textWidth > columnWidth)
            {
              cellText.resize(availableCharacters);
              cellText.resize(availableCharacters + 3, '.');
            }

            drawList->ChannelsSplit(2);

            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign,
                                ImVec2(0, 0.5F));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));
            drawList->ChannelsSetCurrent(1);
            ImGui::Selectable(cellText.c_str(),
                              mData->GetSelectedProject() == project,
                              ImGuiSelectableFlags_SpanAllColumns,
                              ImVec2(0, rowHeight));
            ImGui::PopStyleVar(1);
            ImGui::PopStyleColor(2);

            if (ImGui::IsItemClicked(1))
            {
              mData->SetSelectedProject(project);
            }

            {
              ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
              ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5);
              ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
              ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
              ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
              ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                    IM_COL32(94, 129, 172, 255));
              ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                    IM_COL32(115, 148, 188, 255));
              ImGui::PushStyleColor(ImGuiCol_PopupBg,
                                    IM_COL32(76, 86, 106, 255));
              ImGui::SetNextWindowSize(ImVec2(0, 0));
              if ((mData->GetSelectedProject() == project) &&
                  ImGui::BeginPopupContextItem(
                    "Project options")) // <-- use last item id as popup id
              {
                if (ImGui::Button("Open in file browser",
                                  ImVec2(ImGui::GetContentRegionAvail().x, 0)))
                {
                  mFileHandler->OpenPathInFileBrowser(project.Path);
                  ImGui::CloseCurrentPopup();
                }

                if (ImGui::IsItemHovered())
                {
                  ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }

                if (ImGui::Button("Change Graphics API",
                                  ImVec2(ImGui::GetContentRegionAvail().x, 0)))
                {
                  mData->SetState(ProjectChooserState::ChangeGraphicsApi);
                  mData->SetSelectedProject(project);

                  ImGui::CloseCurrentPopup();
                }

                if (ImGui::IsItemHovered())
                {
                  ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }

                if (ImGui::Button("Remove project from list",
                                  ImVec2(ImGui::GetContentRegionAvail().x, 0)))
                {
                  mSavedProjects->RemoveProject(project.Path);
                  ImGui::CloseCurrentPopup();
                }

                if (ImGui::IsItemHovered())
                {
                  ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }

                ImGui::EndPopup();
              }
              ImGui::PopStyleVar(4);
              ImGui::PopStyleColor(4);
            }

            if (ImGui::IsItemClicked())
            {
              mData->SetSelectedProject(project);
              if (mFileHandler->FileExists(project.Path /
                                           "projectSettings.dproj"))
              {
                mData->SetState(ProjectChooserState::Done);
              }
              else
              {
                // Error dialog that the project doesn't exist anymore with the
                // option to remove the entry
                mData->SetState(ProjectChooserState::ProjectNotFound);
              }
            }

            if (ImGui::IsItemHovered())
            {
              ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
              drawList->ChannelsSetCurrent(0);
              ImVec2 pMin = ImGui::GetItemRectMin();
              ImVec2 pMax = ImGui::GetItemRectMax();
              ImU32  rectCol = ImGui::IsMouseDown(ImGuiMouseButton_Left)
                                 ? IM_COL32(76, 86, 106, 255)
                                 : IM_COL32(67, 76, 94, 255);

              ImGui::GetWindowDrawList()->AddRectFilled(
                pMin, pMax, rectCol, 10);
            }

            drawList->ChannelsMerge();
          }
          else if (column == 1)
          {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (rowHeight / 2) -
                                 (ImGui::GetFontSize() / 2));
            float textWidth =
              ImGui::CalcTextSize(cellText.c_str(), nullptr, false).x;
            float columnWidth = ImGui::GetContentRegionAvail().x - 8;
            int   availableCharacters =
              (int)(columnWidth / (textWidth / cellText.length()));

            if (textWidth > columnWidth)
            {
              cellText.resize(availableCharacters);
              cellText.resize(availableCharacters + 3, '.');
            }

            ImGui::Text("%s", cellText.c_str());
          }
          else
          {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (rowHeight / 2) -
                                 (ImGui::GetFontSize() / 2));
            ImGui::Text("%s", cellText.c_str());
          }
        }
      }
      ImGui::EndTable();
      ImGui::PopFont();
      ImGui::PopStyleVar(2);
      ImGui::PopStyleColor(3);
    }

    for (auto& path : projectsToRemove)
    {
      // mSavedProjects->RemoveProject(path);
    }

    projectsToRemove.clear();

    ImGui::End();
    ImGui::PopStyleColor(2);
  }
}