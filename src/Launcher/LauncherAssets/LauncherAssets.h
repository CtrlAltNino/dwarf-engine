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
    std::shared_ptr<IDwarfLogger>    mLogger;
    std::shared_ptr<ITextureFactory> mTextureFactory;
    /// @brief Font loaded into IMGUI for header text
    std::shared_ptr<ImFont> mHeaderFont;

    /// @brief Font loaded into IMGUI for regular text
    std::shared_ptr<ImFont> mTextFont;

    /// @brief Loaded image for the github icon
    std::shared_ptr<ITexture> mGithubIcon;

    /// @brief Loaded image for the patreon icon
    std::shared_ptr<ITexture> mPatreonIcon;

    /// @brief Loaded image for the X/Twitter icon
    std::shared_ptr<ITexture> mXIcon;

    std::shared_ptr<IWindow> mWindow;

  public:
    LauncherAssets(std::shared_ptr<IDwarfLogger>    logger,
                   std::shared_ptr<ITextureFactory> textureFactory,
                   std::shared_ptr<IWindow>         window);

    /// @brief Font loaded into IMGUI for header text
    auto
    GetHeaderFont() -> std::shared_ptr<ImFont> override;

    /// @brief Font loaded into IMGUI for regular text
    auto
    GetTextFont() -> std::shared_ptr<ImFont> override;

    /// @brief Loaded image for the github icon
    auto
    GetGithubIcon() -> std::shared_ptr<ITexture> override;

    /// @brief Loaded image for the patreon icon
    auto
    GetPatreonIcon() -> std::shared_ptr<ITexture> override;

    /// @brief Loaded image for the X/Twitter icon
    auto
    GetXIcon() -> std::shared_ptr<ITexture> override;
  };
}