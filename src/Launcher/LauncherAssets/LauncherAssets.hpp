#pragma once

#include "Core/Rendering/Texture/ITextureFactory.hpp"
#include "ILauncherAssets.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "Window/IWindow.hpp"

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
  };
}