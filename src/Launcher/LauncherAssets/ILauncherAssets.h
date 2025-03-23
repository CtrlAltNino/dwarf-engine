#pragma once

#include "Core/Rendering/Texture/ITexture.h"
#include <imgui.h>

namespace Dwarf
{
  /**
   * @brief Class providing the assets for the project launcher
   *
   */
  class ILauncherAssets
  {
  public:
    /// @brief Font loaded into IMGUI for header text
    virtual auto
    GetHeaderFont() -> std::shared_ptr<ImFont> = 0;

    /// @brief Font loaded into IMGUI for regular text
    virtual auto
    GetTextFont() -> std::shared_ptr<ImFont> = 0;

    /// @brief Loaded image for the github icon
    virtual auto
    GetGithubIcon() -> std::shared_ptr<ITexture> = 0;

    /// @brief Loaded image for the patreon icon
    virtual auto
    GetPatreonIcon() -> std::shared_ptr<ITexture> = 0;

    /// @brief Loaded image for the X/Twitter icon
    virtual auto
    GetXIcon() -> std::shared_ptr<ITexture> = 0;
  };
}