#pragma once
#include "Launcher/ProjectCreator/IProjectCreator.h"
#include "Launcher/ProjectList/IProjectList.h"
#include "Launcher/ProjectList/IO/IProjectListIO.h"
#include "Launcher/ProjectList/Sorter/IProjectListSorter.h"
#include "pch.h"
#include "Launcher/View/IProjectLauncherView.h"
#include "Launcher/IProjectLauncher.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Window/IWindow.h"
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
    boost::di::extension::lazy<std::shared_ptr<IProjectLauncher>>
                                        m_ProjectLauncher;
    std::shared_ptr<IWindow>            m_Window;
    std::shared_ptr<ITextureFactory>    m_TextureFactory;
    std::shared_ptr<IProjectList>       m_ProjectList;
    std::shared_ptr<IProjectListIO>     m_ProjectListIO;
    std::shared_ptr<IProjectListSorter> m_ProjectListSorter;
    std::shared_ptr<IProjectCreator>    m_ProjectCreator;

    /// @brief Font loaded into IMGUI for header text
    ImFont* m_HeaderFont;

    /// @brief Font loaded into IMGUI for regular text
    ImFont* m_TextFont;

    /// @brief Loaded image for the github icon
    std::shared_ptr<ITexture> m_GithubIcon;

    /// @brief Loaded image for the patreon icon
    std::shared_ptr<ITexture> m_PatreonIcon;

    /// @brief Loaded image for the twitter icon
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

  public:
    ProjectLauncherView(boost::di::extension::lazy<
                          std::shared_ptr<IProjectLauncher>> projectLauncher,
                        std::shared_ptr<IWindow>             window,
                        std::shared_ptr<ITextureFactory>     textureFactory,
                        std::shared_ptr<IProjectList>        projectList,
                        std::shared_ptr<IProjectListIO>      projectListIO,
                        std::shared_ptr<IProjectListSorter>  projectListSorter,
                        std::shared_ptr<IProjectCreator>     projectCreator);

    /**
     * @brief Render the project launcher view
     */
    void
    Render();
  };
}