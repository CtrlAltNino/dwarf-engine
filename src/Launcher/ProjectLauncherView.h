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
    Ref<ProjectLauncherModel> m_Model;

    /// @brief Font loaded into IMGUI for header text
    ImFont* m_HeaderFont;

    /// @brief Font loaded into IMGUI for regular text
    ImFont* m_TextFont;

    /// @brief Loaded image for the github icon
    Ref<Texture> m_GithubIcon;

    /// @brief Loaded image for the patreon icon
    Ref<Texture> m_PatreonIcon;

    /// @brief Loaded image for the twitter icon
    Ref<Texture> m_TwitterIcon;
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
    ProjectLauncherView(Ref<ProjectLauncherModel> model);
    void
    Render();
  };
}