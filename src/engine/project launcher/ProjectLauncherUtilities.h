#pragma once
#include "../../utilities/dpch.h"
#include"../../utilities/Common.h"

/// @brief Enum defining the states of the project launcher.
enum class ProjectChooserState {Choosing, Done, ProjectNotFound, CreateNewProject, ChangeGraphicsApi, Canceled};

/// @brief EEnum defining the available project templates.
enum class ProjectTemplate {Blank, Demo1};
#define GITHUB_PNG_ICON_PATH "data/engine/img/icons/github.png"
#define PATREON_PNG_ICON_PATH "data/engine/img/icons/patreon.png"
#define TWITTER_PNG_ICON_PATH "data/engine/img/icons/twitter.png"
#define APPLICATION_PNG_ICON_PATH "data/engine/img/icons/icon.png"

#define FONT_ROBOTO_REGULAR_PATH "data/engine/fonts/Roboto-Regular.ttf"
#define FONT_ROBOTO_LIGHT_PATH "data/engine/fonts/Roboto-Light.ttf"

#define GITHUB_LINK "https://github.com/flash-miller/simple-3d-engine"
#define PATREON_LINK "https://patreon.com/flash-miller"
#define TWITTER_LINK "https://twitter.com/flash_miller"

/// @brief Structure holding information about a project.
struct ProjectInformation {
	std::string name;
	std::string path;
	int lastOpened;
	int graphicsApi;
};