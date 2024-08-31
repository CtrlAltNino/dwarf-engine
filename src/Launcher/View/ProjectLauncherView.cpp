#include "ProjectLauncherView.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
// #include "Launcher/ProjectListHandler.h"
// #include "Launcher/ProjectSorter.h"
// #include "Launcher/ProjectCreator.h"
// #include "Core/Rendering/TextureCreator.h"
#include "Launcher/IProjectLauncher.h"
#include "Launcher/ProjectList/IProjectList.h"
#include "Launcher/ProjectList/Sorter/IProjectListSorter.h"
#include "Utilities/TimeUtilities.h"
#include "Utilities/BrowserLinkOpener.h"
#include "Core/Base.h"
#include "Utilities/FileHandler.h"
#include <iostream>
#include <nfd.h>

namespace Dwarf
{

#define PROJECT_BUTTON_WINDOW_WIDTH (200)
#define PROJECT_INFORMATION_HEIGHT (30)

  ProjectLauncherView::ProjectLauncherView(
    boost::di::extension::lazy<std::shared_ptr<IProjectLauncher>>
                                        projectLauncher,
    std::shared_ptr<IWindow>            window,
    std::shared_ptr<ITextureFactory>    textureFactory,
    std::shared_ptr<IProjectList>       projectList,
    std::shared_ptr<IProjectListIO>     projectListIO,
    std::shared_ptr<IProjectListSorter> projectListSorter,
    std::shared_ptr<IProjectCreator>    projectCreator)
    : m_Window(window)
    , m_TextureFactory(textureFactory)
    , m_ProjectLauncher(projectLauncher)
    , m_ProjectList(projectList)
    , m_ProjectListIO(projectListIO)
    , m_ProjectListSorter(projectListSorter)
    , m_ProjectCreator(projectCreator)
  {
    ImGuiIO io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    m_HeaderFont = io.Fonts->AddFontFromFileTTF(FONT_ROBOTO_LIGHT_PATH, 26);
    m_TextFont = io.Fonts->AddFontFromFileTTF(FONT_ROBOTO_REGULAR_PATH, 15);

    m_GithubIcon =
      m_TextureFactory->FromPath(std::filesystem::path(GITHUB_PNG_ICON_PATH));
    m_PatreonIcon =
      m_TextureFactory->FromPath(std::filesystem::path(PATREON_PNG_ICON_PATH));
    m_XIcon =
      m_TextureFactory->FromPath(std::filesystem::path(X_PNG_ICON_PATH));
  }

  void
  ProjectLauncherView::Render()
  {
    glm::ivec2 windowSize = { m_Window->GetWidth(), m_Window->GetHeight() };

    RenderProjectList(windowSize);
    RenderButtons(windowSize);
    RenderFooter(windowSize);
    ProjectChooserState state = m_ProjectLauncher.get()->GetState();

    if (state == ProjectChooserState::ProjectNotFound)
    {
      m_ProjectLauncher.get()->SetState(ProjectChooserState::Choosing);
      ImGui::OpenPopup("Project not found!");
    }
    else if (state == ProjectChooserState::CreateNewProject)
    {
      m_ProjectLauncher.get()->SetState(ProjectChooserState::Choosing);
      ImGui::OpenPopup("Create new project");
    }
    else if (state == ProjectChooserState::ChangeGraphicsApi)
    {
      m_ProjectLauncher.get()->SetState(ProjectChooserState::Choosing);
      ImGui::OpenPopup("Change Graphics API");
    }

    RenderProjectNotFoundModal();
    RenderCreateNewProjectModal();
    RenderChangeGraphicsApiModal();
  }

  void
  ProjectLauncherView::RenderProjectList(glm::ivec2 windowSize)
  {
    const std::vector<ProjectInformation>& projectList =
      m_ProjectList->GetProjectList();
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoTitleBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 8));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(windowSize.x - PROJECT_BUTTON_WINDOW_WIDTH,
                                    windowSize.y - PROJECT_INFORMATION_HEIGHT));
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(46, 52, 64, 255));

    if (!ImGui::Begin("Project List", NULL, window_flags))
    {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
    ImGui::PushFont(m_HeaderFont);
    ImGui::Text("Your Projects");
    ImGui::PopFont();
    ImGui::Separator();
    ImGui::PopStyleVar(1);

    const int   COLUMNS_COUNT = 4;
    const float ROW_HEIGHT = 50;
    const float HEADER_ROW_HEIGHT = 40;
    ImVec2      cellPadding = ImVec2(10.0f, 10.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cellPadding);
    ImGui::PushStyleColor(ImGuiCol_TableBorderLight, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, IM_COL32(59, 66, 82, 255));
    if (ImGui::BeginTable("Project entries",
                          COLUMNS_COUNT,
                          ImGuiTableFlags_Borders |
                            ImGuiTableFlags_SizingFixedFit |
                            ImGuiTableFlags_PreciseWidths))
    {
      float initialWidth = ImGui::GetContentRegionAvail().x - 80;
      ImGui::TableSetupColumn("Name",
                              ImGuiTableColumnFlags_NoResize |
                                ImGuiTableColumnFlags_NoReorder |
                                ImGuiTableColumnFlags_WidthFixed,
                              initialWidth * 0.2);
      ImGui::TableSetupColumn("Path",
                              ImGuiTableColumnFlags_NoResize |
                                ImGuiTableColumnFlags_NoReorder |
                                ImGuiTableColumnFlags_WidthFixed,
                              initialWidth * 0.4);
      ImGui::TableSetupColumn("Last opened",
                              ImGuiTableColumnFlags_NoResize |
                                ImGuiTableColumnFlags_NoReorder |
                                ImGuiTableColumnFlags_WidthFixed,
                              initialWidth * 0.25);
      ImGui::TableSetupColumn("API",
                              ImGuiTableColumnFlags_NoResize |
                                ImGuiTableColumnFlags_NoReorder |
                                ImGuiTableColumnFlags_WidthFixed,
                              initialWidth * 0.15);
      // ImGui::Columns(4);
      // ImGui::SetColumnWidth(-1, 50);

      // Dummy entire-column selection storage
      // FIXME: It would be nice to actually demonstrate full-featured selection
      // using those checkbox.
      ImDrawList* draw_list = ImGui::GetWindowDrawList();

      // Instead of calling TableHeadersRow() we'll submit custom headers
      // ourselves
      ImGui::TableNextRow(ImGuiTableRowFlags_Headers, HEADER_ROW_HEIGHT);
      ImGui::PushFont(m_HeaderFont);
      ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(76, 86, 106, 255));
      ImGui::PushStyleColor(ImGuiCol_HeaderActive,
                            IM_COL32(129, 161, 193, 255));
      for (int column = 0; column < COLUMNS_COUNT; column++)
      {
        ImGui::TableSetColumnIndex(column);
        ImGui::PushID(column);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() +
                             (HEADER_ROW_HEIGHT / 2 - cellPadding.y / 2) -
                             ImGui::GetFontSize() / 2);
        ImGui::TableHeader(ImGui::TableGetColumnName(column));

        if (ImGui::IsItemHovered())
        {
          ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        if (ImGui::IsItemClicked())
        {
          m_ProjectListSorter->UpdateSortOrder(ProjectListColumn(column));
          m_ProjectListSorter->SortProjectList(m_ProjectList->GetProjectList());
        }

        ImGui::PopID();
      }
      ImGui::PopStyleColor(2);
      ImGui::PopFont();

      ImGui::PushFont(m_TextFont);
      for (ProjectInformation project : projectList)
      {
        ImGui::TableNextRow(ImGuiTableRowFlags_None, ROW_HEIGHT);
        for (int column = 0;
             (column < COLUMNS_COUNT) /*&& (row < projectList.size())*/;
             column++)
        {
          ImGui::TableSetColumnIndex(column);
          std::string cellText = "";
          switch (column)
          {
            case 0: cellText = project.name; break;
            case 1: cellText = project.path.t.string(); break;
            case 2:
              {
                time_t lastOpenedTime = project.lastOpened;
                cellText =
                  lastOpenedTime == -1
                    ? "never"
                    : Dwarf::TimeUtilities::CalculateTimePassedSinceNow(
                        project.lastOpened);
                break;
              }
            case 3:
              cellText = graphicsApiNames[(int)project.graphicsApi];
              break;
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

            draw_list->ChannelsSplit(2);

            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign,
                                ImVec2(0, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));
            draw_list->ChannelsSetCurrent(1);
            ImGui::Selectable(cellText.c_str(),
                              m_ProjectLauncher.get()->GetSelectedProject() ==
                                project,
                              ImGuiSelectableFlags_SpanAllColumns,
                              ImVec2(0, ROW_HEIGHT));
            ImGui::PopStyleVar(1);
            ImGui::PopStyleColor(2);

            if (ImGui::IsItemClicked(1))
            {
              m_ProjectLauncher.get()->SetSelectedProject(project);
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
              if ((m_ProjectLauncher.get()->GetSelectedProject() == project) &&
                  ImGui::BeginPopupContextItem(
                    "Project options")) // <-- use last item id as popup id
              {
                if (ImGui::Button("Open in file browser",
                                  ImVec2(ImGui::GetContentRegionAvail().x, 0)))
                {
                  FileHandler::OpenPathInFileBrowser(project.path);
                  ImGui::CloseCurrentPopup();
                }

                if (ImGui::IsItemHovered())
                  ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

                if (ImGui::Button("Change Graphics API",
                                  ImVec2(ImGui::GetContentRegionAvail().x, 0)))
                {
                  m_ProjectLauncher.get()->SetState(
                    ProjectChooserState::ChangeGraphicsApi);
                  m_ProjectLauncher.get()->SetSelectedProject(project);

                  ImGui::CloseCurrentPopup();
                }

                if (ImGui::IsItemHovered())
                  ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

                if (ImGui::Button("Remove project from list",
                                  ImVec2(ImGui::GetContentRegionAvail().x, 0)))
                {
                  m_ProjectList->RemoveProject(project.path);
                  ImGui::CloseCurrentPopup();
                }

                if (ImGui::IsItemHovered())
                  ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

                ImGui::EndPopup();
              }
              ImGui::PopStyleVar(4);
              ImGui::PopStyleColor(4);
            }

            if (ImGui::IsItemClicked())
            {
              m_ProjectLauncher.get()->SetSelectedProject(project);
              if (FileHandler::FileExists(project.path.t /
                                          "projectSettings.dproj"))
              {
                m_ProjectLauncher.get()->SetState(ProjectChooserState::Done);
              }
              else
              {
                // Error dialog that the project doesn't exist anymore with the
                // option to remove the entry
                m_ProjectLauncher.get()->SetState(
                  ProjectChooserState::ProjectNotFound);
              }
            }

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
                p_min, p_max, rectCol, 10);
            }

            draw_list->ChannelsMerge();
          }
          else if (column == 1)
          {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (ROW_HEIGHT / 2) -
                                 ImGui::GetFontSize() / 2);
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
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (ROW_HEIGHT / 2) -
                                 ImGui::GetFontSize() / 2);
            ImGui::Text("%s", cellText.c_str());
          }
        }
      }
      ImGui::EndTable();
      ImGui::PopFont();
      ImGui::PopStyleVar(2);
      ImGui::PopStyleColor(3);
    }

    ImGui::End();
    ImGui::PopStyleColor(2);
  }

  void
  ProjectLauncherView::RenderButtons(glm::ivec2 windowSize)
  {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoTitleBar;

    ImGui::SetNextWindowPos(
      ImVec2(windowSize.x - PROJECT_BUTTON_WINDOW_WIDTH, 0));
    ImGui::SetNextWindowSize(ImVec2(PROJECT_BUTTON_WINDOW_WIDTH,
                                    windowSize.y - PROJECT_INFORMATION_HEIGHT));
    ImGui::PushStyleColor(ImGuiCol_WindowBg,
                          ImVec4(67 / 255.0, 76 / 255.0, 94 / 255.0, 1.0));
    ImGui::PushStyleColor(ImGuiCol_Border,
                          ImVec4(59 / 255.0, 66 / 255.0, 82 / 255.0, 0));

    if (!ImGui::Begin("Project Buttons", NULL, window_flags))
    {
      ImGui::End();
      return;
    }

    ImGui::PushFont(m_HeaderFont);
    ImGui::Text("Add Project");
    ImGui::PopFont();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
    ImGui::Separator();
    ImGui::PushFont(m_TextFont);
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
      // TODO: Open modal for creating a new project
      m_ProjectLauncher.get()->SetState(ProjectChooserState::CreateNewProject);
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
      m_ProjectList->OpenAddProjectDialog();
    }

    if (ImGui::IsItemHovered())
    {
      ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);

    ImGui::PushFont(m_HeaderFont);
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

  void
  ProjectLauncherView::RenderFooter(glm::ivec2 windowSize)
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
    ImGui::PushFont(m_TextFont);

    if (!ImGui::Begin("Information", NULL, window_flags))
    {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      return;
    }

    ImGui::Text("An open source project created by flash-miller");
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
      ImTextureID texID = (ImTextureID)m_GithubIcon->GetTextureID();
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
      ImTextureID texID = (ImTextureID)m_PatreonIcon->GetTextureID();
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
      ImTextureID texID = (ImTextureID)m_XIcon->GetTextureID();
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
      ImGui::Text("Twitter");
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

  void
  ProjectLauncherView::RenderChangeGraphicsApiModal()
  {
    // Centering Modal
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGuiStyle* style = &ImGui::GetStyle();
    (void)style;
    style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.7);

    // Setting the font for the modal window title
    ImGui::PushFont(m_HeaderFont);
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
      ImGui::PushFont(m_TextFont);
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
        (int)m_ProjectLauncher.get()->GetSelectedProject().graphicsApi;
      // ==================== Graphics API Selection Dropdown
      // ====================
      {
        // Graphics Title
        ImGui::PushFont(m_HeaderFont);
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
        ImGui::PushFont(m_TextFont);

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

        if (ImGui::BeginCombo("##graphicsApi",
                              graphicsApiNames[currentApiIndex].data()))
        {
          ImDrawList* draw_list = ImGui::GetWindowDrawList();

          // Looping through all the combo entries
          for (int n = 0;
               n < sizeof(graphicsApiNames) / sizeof(graphicsApiNames[0]);
               n++)
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

              // ==================== Graphics Selectable ====================
              if (ImGui::Selectable(graphicsApiNames[n].data(),
                                    is_selected,
                                    0,
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
        m_ProjectList->ChangeGraphicsApi(
          m_ProjectLauncher.get()->GetSelectedProject().path,
          (GraphicsApi)currentApiIndex);
        ImGui::CloseCurrentPopup();
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

  void
  ProjectLauncherView::RenderProjectNotFoundModal()
  {
    // Centering Modal
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    // Setting the dimming background color
    ImGuiStyle* style = &ImGui::GetStyle();
    (void)style;
    style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.7);

    // Setting the font for the modal window title
    ImGui::PushFont(m_HeaderFont);
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
      ImGui::PushFont(m_TextFont);
      // ==================== Information Text ====================
      float textWidth =
        ImGui::CalcTextSize(
          m_ProjectLauncher.get()->GetSelectedProject().path.t.string().c_str(),
          nullptr,
          false)
          .x;

      ImGui::Text("The project you are trying to open could not be found:");

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
      ImGui::SetCursorPosX(
        ImGui::GetCursorPosX() +
        (ImGui::GetContentRegionAvail().x / 2 - textWidth / 2));
      ImGui::Text(
        "%s",
        m_ProjectLauncher.get()->GetSelectedProject().path.t.string().c_str());

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
                        ImVec2(ImGui::GetContentRegionAvail().x / 2 - 10, 0)))
      {
        m_ProjectList->RemoveProject(
          m_ProjectLauncher.get()->GetSelectedProject().path);
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

  void
  ProjectLauncherView::RenderCreateNewProjectModal()
  {
    // Centering Modal ====================
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    // Setting the font for the modal window title
    ImGui::PushFont(m_HeaderFont);
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

      static char newProjectName[128] = "";
      // ==================== Name Input ====================
      {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
        // Title
        ImGui::PushFont(m_HeaderFont);
        ImGui::Text("Name");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                             ImGui::GetContentRegionAvail().x - 200);

        // Text Input
        ImGui::PushFont(m_TextFont);
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(59, 66, 82, 255));
        ImGui::InputTextWithHint("##projectName",
                                 "Enter name",
                                 newProjectName,
                                 IM_ARRAYSIZE(newProjectName));
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor();
        ImGui::PopItemWidth();
        ImGui::PopFont();
      }

      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
      ImGui::Separator();
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

      static std::filesystem::path newProjectPath =
        m_ProjectCreator->GetDefaultProjectPath();
      // ==================== Project Path Selector ====================
      {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        // Title
        ImGui::PushFont(m_HeaderFont);
        ImGui::Text("Path");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                             ImGui::GetContentRegionAvail().x - 300);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

        draw_list->ChannelsSplit(2);
        draw_list->ChannelsSetCurrent(1);
        // Path preview
        ImGui::PushFont(m_TextFont);
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
          // Directory Dialog
          // File Dialog um einen Pfad zu kriegen
          // Im Pfad nach einer projectSettings.dproj suchen
          // Projectinformation (Name, Pfad, letzte Modifikationszeit)

          nfdu8char_t*          outPath = NULL;
          nfdpickfolderu8args_t args = { 0 };
          args.defaultPath = (const nfdu8char_t*)newProjectPath.c_str();
          // nfdu8
          nfdresult_t result = NFD_PickFolderU8_With(&outPath, &args);

          if (result == NFD_OKAY)
          {
            newProjectPath = std::string(outPath);
            free(outPath);
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
        ImGui::PushFont(m_HeaderFont);
        ImGui::Text("Template");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                             ImGui::GetContentRegionAvail().x - 150);

        // Setting up combo
        std::vector<std::string> templates = { "Blank", "Sponza" };

        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
        ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign,
                            ImVec2(0.5f, 0.5f));
        ImGui::PushFont(m_TextFont);

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
          for (int n = 0; n < 2; n++)
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
        ImGui::PushFont(m_HeaderFont);
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
        ImGui::PushFont(m_TextFont);

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

        if (ImGui::BeginCombo("##graphicsApi",
                              graphicsApiNames[currentApiIndex].data()))
        {
          ImDrawList* draw_list = ImGui::GetWindowDrawList();

          // Looping through all the combo entries
          for (int n = 0;
               n < sizeof(graphicsApiNames) / sizeof(graphicsApiNames[0]);
               n++)
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

              // ==================== Rendering Selectable ====================
              if (ImGui::Selectable(graphicsApiNames[n].data(),
                                    is_selected,
                                    0,
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

      ImGui::PushStyleColor(ImGuiCol_Button,
                            ImVec4(76 / 255.0, 86 / 255.0, 106 / 255.0, 1));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            ImVec4(86 / 255.0, 95 / 255.0, 114 / 255.0, 1));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                            ImVec4(129 / 255.0, 161 / 255.0, 193 / 255.0, 1));
      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

      // ==================== Create Button ====================
      {
        ImGui::PushFont(m_TextFont);
        if (ImGui::Button("Create",
                          ImVec2(ImGui::GetContentRegionAvail().x / 2 - 8, 40)))
        {
          // Create folder at the path with the project name
          // Create and fill projectSettings.dproj appropriately
          // If using a template, copy the corresponding files
          // Add project to saved project list
          // Launch project after creating, or just close the popup?
          // CreateProject(newProjectName, newProjectPath.c_str(),
          // (GraphicsApi)currentApiIndex,
          // (ProjectTemplate)currentTemplateIndex);
          m_ProjectCreator->CreateProject(
            newProjectName,
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
        ImGui::PushFont(m_TextFont);
        if (ImGui::Button("Cancel",
                          ImVec2(ImGui::GetContentRegionAvail().x, 40)))
        {
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