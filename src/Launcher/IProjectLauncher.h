#pragma once

#include "SavedProjects/ISavedProjects.h"

namespace Dwarf
{
#define GITHUB_PNG_ICON_PATH "data/engine/img/icons/github.png"
#define PATREON_PNG_ICON_PATH "data/engine/img/icons/patreon.png"
#define X_PNG_ICON_PATH "data/engine/img/icons/x.png"
#define APPLICATION_PNG_ICON_PATH "data/engine/img/icons/icon.png"

#define FONT_ROBOTO_REGULAR_PATH "data/engine/fonts/Roboto-Regular.ttf"
#define FONT_ROBOTO_LIGHT_PATH "data/engine/fonts/Roboto-Light.ttf"

#define GITHUB_LINK "https://github.com/CtrlAltNino/dwarf-engine"
#define PATREON_LINK "https://patreon.com/CtrlAltNino"
#define X_LINK "https://x.com/CtrlAltNino"

  /// @brief Enum defining the states of the project launcher.
  enum class ProjectChooserState
  {
    Choosing,
    Done,
    ProjectNotFound,
    CreateNewProject,
    ChangeGraphicsApi,
    Cancelled
  };

  /**
   * @brief Class representing the project launcher
   *
   */
  class IProjectLauncher
  {
  public:
    virtual ~IProjectLauncher() = default;

    /**
     * @brief Runs the project launcher
     *
     * @return If the user chose a project, the saved project data will be
     * returned
     */
    virtual auto
    Run() -> std::optional<SavedProject> = 0;
  };
}