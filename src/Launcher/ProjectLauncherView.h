#pragma once

#include "Core/Base.h"
#include "ProjectLauncherUtilities.h"
#include "ProjectLauncherModel.h"
#include "Utilities/TimeUtilities.h"
#include "Utilities/BrowserLinkOpener.h"
#include "Core/Rendering/Texture.h"
#include "Core/UI/ImGuiLayer.h"

#include "imgui.h"

namespace Dwarf
{

  class ProjectLauncherView
  {
  private:
    std::shared_ptr<ProjectLauncherModel> m_Model;

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
    void
    RenderProjectList(int fWidth, int fHeight);

    void
    RenderButtons(int fWidth, int fHeight);

    void
    RenderFooter(int fWidth, int fHeight);

    void
    RenderChangeGraphicsApiModal();

    void
    RenderProjectNotFoundModal();

    void
    RenderCreateNewProjectModal();

  public:
    ProjectLauncherView(std::shared_ptr<ProjectLauncherModel> model);
    void
    Render();
  };
}