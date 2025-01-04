#pragma once
#include "Launcher/IProjectLauncherData.h"
#include "Launcher/ProjectCreator/IProjectCreator.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Launcher/SavedProjects/IO/ISavedProjectsIO.h"
#include "Launcher/SavedProjects/Sorter/ISavedProjectsSorter.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include "pch.h"
#include "Launcher/View/IProjectLauncherView.h"
#include "Launcher/IProjectLauncher.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Window/IWindow.h"
#include <imgui.h>
#include <boost/di/extension/injections/lazy.hpp>
#include <string>

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
    std::shared_ptr<IDwarfLogger>         m_Logger;
    std::shared_ptr<IWindow>              m_Window;
    std::shared_ptr<IProjectLauncherData> m_Data;
    std::shared_ptr<ITextureFactory>      m_TextureFactory;
    std::shared_ptr<ISavedProjects>       m_SavedProjects;
    std::shared_ptr<ISavedProjectsIO>     m_SavedProjectsIO;
    std::shared_ptr<ISavedProjectsSorter> m_SavedProjectsSorter;
    std::shared_ptr<IProjectCreator>      m_ProjectCreator;
    std::shared_ptr<IFileHandler>         m_FileHandler;

    /// @brief Font loaded into IMGUI for header text
    std::shared_ptr<ImFont> m_HeaderFont;

    /// @brief Font loaded into IMGUI for regular text
    std::shared_ptr<ImFont> m_TextFont;

    /// @brief Loaded image for the github icon
    std::shared_ptr<ITexture> m_GithubIcon;

    /// @brief Loaded image for the patreon icon
    std::shared_ptr<ITexture> m_PatreonIcon;

    /// @brief Loaded image for the X/Twitter icon
    std::shared_ptr<ITexture> m_XIcon;

    /**
     * @brief Render the project list
     * @param windowSize Size of the window
     */
    void
    RenderProjectList(glm::ivec2 windowSize);

    /**
     * @brief Render a button
     * @param windowSize Size of the window
     */
    void
    RenderButtons(glm::ivec2 windowSize);

    /**
     * @brief Render the footer
     * @param windowSize Size of the window
     */
    void
    RenderFooter(glm::ivec2 windowSize);

    /**
     * @brief Render the change graphics API modal
     */
    void
    RenderChangeGraphicsApiModal();

    /**
     * @brief Render the project not found modal
     */
    void
    RenderProjectNotFoundModal();

    /**
     * @brief Render the create new project modal
     */
    void
    RenderCreateNewProjectModal();

    std::string
    GetGraphicsApiString(GraphicsApi api);

  public:
    ProjectLauncherView(
      std::shared_ptr<IDwarfLogger>         logger,
      std::shared_ptr<IWindow>              window,
      std::shared_ptr<IProjectLauncherData> data,
      std::shared_ptr<ITextureFactory>      textureFactory,
      std::shared_ptr<ISavedProjects>       savedProjects,
      std::shared_ptr<ISavedProjectsIO>     savedProjectsIO,
      std::shared_ptr<ISavedProjectsSorter> savedProjectsSorter,
      std::shared_ptr<IProjectCreator>      projectCreator,
      std::shared_ptr<IFileHandler>         fileHandler);

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