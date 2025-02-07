#include "Core/Base.h"

#include "ProjectLauncherView.h"

#include "ButtonsView/IButtonsView.h"
#include "ChangeGraphicsApiModal/IChangeGraphicsApiModal.h"
#include "Launcher/IProjectLauncher.h"
#include "ProjectListView/IProjectListView.h"
#include "ProjectNotFoundModal/IProjectNotFoundModal.h"

#include <imgui.h>
#include <nfd.h>

namespace Dwarf
{

#define PROJECT_BUTTON_WINDOW_WIDTH (200)
#define PROJECT_INFORMATION_HEIGHT (30)

  ProjectLauncherView::ProjectLauncherView(
    std::shared_ptr<IDwarfLogger>            logger,
    std::shared_ptr<IWindow>                 window,
    std::shared_ptr<IProjectLauncherData>    data,
    std::shared_ptr<ISavedProjects>          savedProjects,
    std::shared_ptr<ISavedProjectsIO>        savedProjectsIO,
    std::shared_ptr<ISavedProjectsSorter>    savedProjectsSorter,
    std::shared_ptr<IProjectCreator>         projectCreator,
    std::shared_ptr<IFileHandler>            fileHandler,
    std::shared_ptr<IButtonsView>            buttonsView,
    std::shared_ptr<IFooterView>             footerView,
    std::shared_ptr<IProjectListView>        projectListView,
    std::shared_ptr<IChangeGraphicsApiModal> changeGraphicsApiModal,
    std::shared_ptr<IProjectNotFoundModal>   projectNotFoundModal,
    std::shared_ptr<ICreateNewProjectModal>  createNewProjectModal)
    : m_Window(window)
    , m_Data(data)
    , m_SavedProjects(savedProjects)
    , m_SavedProjectsIO(savedProjectsIO)
    , m_SavedProjectsSorter(savedProjectsSorter)
    , m_ProjectCreator(projectCreator)
    , m_Logger(logger)
    , m_FileHandler(fileHandler)
    , m_ButtonsView(buttonsView)
    , m_FooterView(footerView)
    , m_ProjectListView(projectListView)
    , m_ChangeGraphicsApiModal(changeGraphicsApiModal)
    , m_ProjectNotFoundModal(projectNotFoundModal)
    , m_CreateNewProjectModal(createNewProjectModal)
  {
    m_Logger->LogDebug(
      Log("Initializing project launcher view", "ProjectLauncherView"));

    m_Logger->LogDebug(
      Log("Project launcher view initialized", "ProjectLauncherView"));
  }

  ProjectLauncherView::~ProjectLauncherView()
  {
    m_Logger->LogDebug(
      Log("Destroying project launcher view", "ProjectLauncherView"));
  }

  void
  ProjectLauncherView::Show()
  {
    m_Window->ShowWindow();
  }

  void
  ProjectLauncherView::Render()
  {
    if (m_Window->ShouldClose())
    {
      m_Data->SetState(ProjectChooserState::Cancelled);
    }

    m_Window->NewFrame();

    glm::ivec2 windowSize = { m_Window->GetWidth(), m_Window->GetHeight() };

    m_ProjectListView->RenderProjectList(windowSize);
    m_ButtonsView->RenderButtons(windowSize);
    m_FooterView->RenderFooter(windowSize);
    ProjectChooserState state = m_Data->GetState();

    if (state == ProjectChooserState::ProjectNotFound)
    {
      m_Data->SetState(ProjectChooserState::Choosing);
      ImGui::OpenPopup("Project not found!");
    }
    else if (state == ProjectChooserState::CreateNewProject)
    {
      m_Data->SetState(ProjectChooserState::Choosing);
      ImGui::OpenPopup("Create new project");
    }
    else if (state == ProjectChooserState::ChangeGraphicsApi)
    {
      m_Data->SetState(ProjectChooserState::Choosing);
      ImGui::OpenPopup("Change Graphics API");
    }

    m_ProjectNotFoundModal->RenderProjectNotFoundModal();
    m_CreateNewProjectModal->RenderCreateNewProjectModal();
    m_ChangeGraphicsApiModal->RenderChangeGraphicsApiModal();

    m_Window->EndFrame();
  }
}