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
    std::shared_ptr<ILauncherData>           data,
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
    : mWindow(window)
    , mData(data)
    , mSavedProjects(savedProjects)
    , mSavedProjectsIO(savedProjectsIO)
    , mSavedProjectsSorter(savedProjectsSorter)
    , mProjectCreator(projectCreator)
    , mLogger(logger)
    , mFileHandler(fileHandler)
    , mButtonsView(buttonsView)
    , mFooterView(footerView)
    , mProjectListView(projectListView)
    , mChangeGraphicsApiModal(changeGraphicsApiModal)
    , mProjectNotFoundModal(projectNotFoundModal)
    , mCreateNewProjectModal(createNewProjectModal)
  {
    mLogger->LogDebug(
      Log("Initializing project launcher view", "ProjectLauncherView"));

    mLogger->LogDebug(
      Log("Project launcher view initialized", "ProjectLauncherView"));
  }

  ProjectLauncherView::~ProjectLauncherView()
  {
    mLogger->LogDebug(
      Log("Destroying project launcher view", "ProjectLauncherView"));
  }

  void
  ProjectLauncherView::Show()
  {
    mWindow->showWindow();
  }

  void
  ProjectLauncherView::Render()
  {
    if (mWindow->shouldClose())
    {
      mData->SetState(ProjectChooserState::Cancelled);
    }

    mWindow->newFrame();

    glm::ivec2 windowSize = { mWindow->getWidth(), mWindow->getHeight() };

    mProjectListView->RenderProjectList(windowSize);
    mButtonsView->RenderButtons(windowSize);
    mFooterView->RenderFooter(windowSize);
    ProjectChooserState state = mData->GetState();

    if (state == ProjectChooserState::ProjectNotFound)
    {
      mData->SetState(ProjectChooserState::Choosing);
      ImGui::OpenPopup("Project not found!");
    }
    else if (state == ProjectChooserState::CreateNewProject)
    {
      mData->SetState(ProjectChooserState::Choosing);
      ImGui::OpenPopup("Create new project");
    }
    else if (state == ProjectChooserState::ChangeGraphicsApi)
    {
      mData->SetState(ProjectChooserState::Choosing);
      ImGui::OpenPopup("Change Graphics API");
    }

    mProjectNotFoundModal->RenderProjectNotFoundModal();
    mCreateNewProjectModal->RenderCreateNewProjectModal();
    mChangeGraphicsApiModal->RenderChangeGraphicsApiModal();

    mWindow->endFrame();
  }
}