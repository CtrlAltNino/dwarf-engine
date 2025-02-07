#pragma once

#include "Core/Rendering/Texture/ITexture.h"
#include <imgui.h>
namespace Dwarf
{
  class ILauncherAssets
  {
  public:
    /// @brief Font loaded into IMGUI for header text
    virtual std::shared_ptr<ImFont>
    GetHeaderFont() = 0;

    /// @brief Font loaded into IMGUI for regular text
    virtual std::shared_ptr<ImFont>
    GetTextFont() = 0;

    /// @brief Loaded image for the github icon
    virtual std::shared_ptr<ITexture>
    GetGithubIcon() = 0;

    /// @brief Loaded image for the patreon icon
    virtual std::shared_ptr<ITexture>
    GetPatreonIcon() = 0;

    /// @brief Loaded image for the X/Twitter icon
    virtual std::shared_ptr<ITexture>
    GetXIcon() = 0;
  };
}