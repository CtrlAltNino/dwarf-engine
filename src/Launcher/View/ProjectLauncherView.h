#pragma once

#include "ButtonsView/IButtonsView.h"
#include "ChangeGraphicsApiModal/IChangeGraphicsApiModal.h"
#include "CreateNewProjectModal/ICreateNewProjectModal.h"
#include "FooterView/IFooterView.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"
#include "Launcher/LauncherData/ILauncherData.h"
#include "Launcher/ProjectCreator/IProjectCreator.h"
#include "Launcher/SavedProjects/IO/ISavedProjectsIO.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Launcher/SavedProjects/Sorter/ISavedProjectsSorter.h"
#include "Launcher/View/IProjectLauncherView.h"
#include "Logging/IDwarfLogger.h"
#include "ProjectListView/IProjectListView.h"
#include "ProjectNotFoundModal/IProjectNotFoundModal.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include "Window/IWindow.h"
#include <boost/di/extension/injections/lazy.hpp>
#include <imgui.h>

namespace Dwarf
{
  /**
   * @brief View for the project launcher
   */
  class ProjectLauncherView : public IProjectLauncherView
  {
  private:
    /// @brief Model for the project launcher
    // std::shared_ptr<IProjectLauncherModel> mModel;
    /// @brief Window to render the project launcher in
    std::shared_ptr<IDwarfLogger>            mLogger;
    std::shared_ptr<IWindow>                 mWindow;
    std::shared_ptr<ILauncherData>           mData;
    std::shared_ptr<ISavedProjects>          mSavedProjects;
    std::shared_ptr<ISavedProjectsIO>        mSavedProjectsIO;
    std::shared_ptr<ISavedProjectsSorter>    mSavedProjectsSorter;
    std::shared_ptr<IProjectCreator>         mProjectCreator;
    std::shared_ptr<IFileHandler>            mFileHandler;
    std::shared_ptr<ILauncherAssets>         mLauncherAssets;
    std::shared_ptr<IButtonsView>            mButtonsView;
    std::shared_ptr<IFooterView>             mFooterView;
    std::shared_ptr<IProjectListView>        mProjectListView;
    std::shared_ptr<IChangeGraphicsApiModal> mChangeGraphicsApiModal;
    std::shared_ptr<IProjectNotFoundModal>   mProjectNotFoundModal;
    std::shared_ptr<ICreateNewProjectModal>  mCreateNewProjectModal;

  public:
    ProjectLauncherView(
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
      std::shared_ptr<ICreateNewProjectModal>  createNewProjectModal);

    ~ProjectLauncherView() override;

    /**
     * @brief Show the project launcher view
     */
    void
    Show() override;

    /**
     * @brief Render the project launcher view
     */
    void
    Render() override;
  };
}