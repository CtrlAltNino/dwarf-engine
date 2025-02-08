#pragma once
#include "pch.h"

#include "Launcher/LauncherData/ILauncherData.h"
#include "ButtonsView/IButtonsView.h"
#include "ChangeGraphicsApiModal/IChangeGraphicsApiModal.h"
#include "CreateNewProjectModal/ICreateNewProjectModal.h"
#include "FooterView/IFooterView.h"
#include "Launcher/ProjectCreator/IProjectCreator.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Launcher/SavedProjects/IO/ISavedProjectsIO.h"
#include "Launcher/SavedProjects/Sorter/ISavedProjectsSorter.h"
#include "Logging/IDwarfLogger.h"
#include "ProjectListView/IProjectListView.h"
#include "ProjectNotFoundModal/IProjectNotFoundModal.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include "Launcher/View/IProjectLauncherView.h"
#include "Window/IWindow.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"

#include <imgui.h>
#include <boost/di/extension/injections/lazy.hpp>

namespace Dwarf
{
  /**
   * @brief View for the project launcher
   */
  class ProjectLauncherView : public IProjectLauncherView
  {
  private:
    /// @brief Model for the project launcher
    // std::shared_ptr<IProjectLauncherModel> m_Model;
    /// @brief Window to render the project launcher in
    std::shared_ptr<IDwarfLogger>            m_Logger;
    std::shared_ptr<IWindow>                 m_Window;
    std::shared_ptr<ILauncherData>           m_Data;
    std::shared_ptr<ISavedProjects>          m_SavedProjects;
    std::shared_ptr<ISavedProjectsIO>        m_SavedProjectsIO;
    std::shared_ptr<ISavedProjectsSorter>    m_SavedProjectsSorter;
    std::shared_ptr<IProjectCreator>         m_ProjectCreator;
    std::shared_ptr<IFileHandler>            m_FileHandler;
    std::shared_ptr<ILauncherAssets>         m_LauncherAssets;
    std::shared_ptr<IButtonsView>            m_ButtonsView;
    std::shared_ptr<IFooterView>             m_FooterView;
    std::shared_ptr<IProjectListView>        m_ProjectListView;
    std::shared_ptr<IChangeGraphicsApiModal> m_ChangeGraphicsApiModal;
    std::shared_ptr<IProjectNotFoundModal>   m_ProjectNotFoundModal;
    std::shared_ptr<ICreateNewProjectModal>  m_CreateNewProjectModal;

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