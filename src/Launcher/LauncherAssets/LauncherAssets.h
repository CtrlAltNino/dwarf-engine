#pragma once

#include "Core/Rendering/Texture/ITextureFactory.h"
#include "ILauncherAssets.h"
#include "Logging/IDwarfLogger.h"
#include "Window/IWindow.h"

namespace Dwarf
{
  class LauncherAssets : public ILauncherAssets
  {
  private:
    std::shared_ptr<IDwarfLogger>    m_Logger;
    std::shared_ptr<ITextureFactory> m_TextureFactory;
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

    std::shared_ptr<IWindow> m_Window;

  public:
    LauncherAssets(std::shared_ptr<IDwarfLogger>    logger,
                   std::shared_ptr<ITextureFactory> textureFactory,
                   std::shared_ptr<IWindow>         window);

    /// @brief Font loaded into IMGUI for header text
    std::shared_ptr<ImFont>
    GetHeaderFont() override;

    /// @brief Font loaded into IMGUI for regular text
    std::shared_ptr<ImFont>
    GetTextFont() override;

    /// @brief Loaded image for the github icon
    std::shared_ptr<ITexture>
    GetGithubIcon() override;

    /// @brief Loaded image for the patreon icon
    std::shared_ptr<ITexture>
    GetPatreonIcon() override;

    /// @brief Loaded image for the X/Twitter icon
    std::shared_ptr<ITexture>
    GetXIcon() override;
  };
}