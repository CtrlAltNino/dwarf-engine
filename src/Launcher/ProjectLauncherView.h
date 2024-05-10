#pragma once

#include "Core/Base.h"
#include "IProjectLauncherModel.h"
#include "Core/Rendering/Texture.h"
#include "Launcher/IProjectLauncherView.h"
#include "Window/Window.h"
#include "imgui.h"
#include <memory>

namespace Dwarf
{
  /**
   * @brief View for the project launcher
   */
  class ProjectLauncherView : public IProjectLauncherView
  {
  private:
    /// @brief Model for the project launcher
    const std::unique_ptr<IProjectLauncherModel>& m_Model;
    /// @brief Window to render the project launcher in
    const std::unique_ptr<Window>& m_Window;

    /// @brief Font loaded into IMGUI for header text
    ImFont* m_HeaderFont;

    /// @brief Font loaded into IMGUI for regular text
    ImFont* m_TextFont;

    /// @brief Loaded image for the github icon
    std::shared_ptr<Texture> m_GithubIcon;

    /// @brief Loaded image for the patreon icon
    std::shared_ptr<Texture> m_PatreonIcon;

    /// @brief Loaded image for the twitter icon
    std::shared_ptr<Texture> m_TwitterIcon;

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
    ProjectLauncherView(const std::unique_ptr<IProjectLauncherModel>& model,
                        const std::unique_ptr<Window>&                window);

    /**
     * @brief Render the project launcher view
     */
    void
    Render();
  };
}