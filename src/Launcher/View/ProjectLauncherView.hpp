#pragma once

#include "ButtonsView/IButtonsView.hpp"
#include "ChangeGraphicsApiModal/IChangeGraphicsApiModal.hpp"
#include "CreateNewProjectModal/ICreateNewProjectModal.hpp"
#include "FooterView/IFooterView.hpp"
#include "Launcher/LauncherAssets/ILauncherAssets.hpp"
#include "Launcher/LauncherData/ILauncherData.hpp"
#include "Launcher/ProjectCreator/IProjectCreator.hpp"
#include "Launcher/SavedProjects/IO/ISavedProjectsIO.hpp"
#include "Launcher/SavedProjects/ISavedProjects.hpp"
#include "Launcher/SavedProjects/Sorter/ISavedProjectsSorter.hpp"
#include "Launcher/View/IProjectLauncherView.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "ProjectListView/IProjectListView.hpp"
#include "ProjectNotFoundModal/IProjectNotFoundModal.hpp"
#include "Utilities/FileHandler/IFileHandler.hpp"
#include "Window/IWindow.hpp"
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