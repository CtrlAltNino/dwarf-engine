#pragma once

#include "Core/Base.h"
#include <filesystem>
namespace Dwarf
{

  /// @brief Enum defining the states of the project launcher.
  enum class ProjectChooserState
  {
    Choosing,
    Done,
    ProjectNotFound,
    CreateNewProject,
    ChangeGraphicsApi,
    Canceled
  };

  /// @brief Enum defining the available project templates.
  enum class ProjectTemplate
  {
    Blank,
    Sponza
  };
#define GITHUB_PNG_ICON_PATH "data/engine/img/icons/github.png"
#define PATREON_PNG_ICON_PATH "data/engine/img/icons/patreon.png"
#define TWITTER_PNG_ICON_PATH "data/engine/img/icons/twitter.png"
#define APPLICATION_PNG_ICON_PATH "data/engine/img/icons/icon.png"

#define FONT_ROBOTO_REGULAR_PATH "data/engine/fonts/Roboto-Regular.ttf"
#define FONT_ROBOTO_LIGHT_PATH "data/engine/fonts/Roboto-Light.ttf"

#define GITHUB_LINK "https://github.com/CtrlAltNino/dwarf-engine"
#define PATREON_LINK "https://patreon.com/CtrlAltNino"
#define TWITTER_LINK "https://twitter.com/CtrlAltNino"

  /// @brief Structure holding information about a project.
  struct ProjectInformation
  {
    std::string           name;
    std::filesystem::path path;
    int                   lastOpened;
    GraphicsApi           graphicsApi;
  };
}