#pragma once
#include "Project/ProjectTypes.h"
#include "pch.h"
#include "Core/Base.h"
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
    Canceled
  };

  /// @brief Structure holding information about a project.
  struct ProjectInformation
  {
    std::string name;
    ProjectPath path;
    int         lastOpened;
    GraphicsApi graphicsApi;

    // Equality operator for ProjectInformation.
    bool
    operator==(const ProjectInformation& other) const
    {
      return name == other.name && path == other.path &&
             lastOpened == other.lastOpened && graphicsApi == other.graphicsApi;
    }
  };

  class IProjectLauncher
  {
  public:
    virtual ProjectInformation
    Run() = 0;
  };
}