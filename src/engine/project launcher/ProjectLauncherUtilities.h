#pragma once
#include<string>

enum class GraphicsApi {OpenGL, D3D11, D3D12, Vulkan};
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

struct ProjectInformation {
	std::string name;
	std::string path;
	int lastOpened;
	int graphicsApi;
};

struct ProjectData {
	std::string name;
	std::string path;
	GraphicsApi graphicsApi;
};

static const char* graphicsApiNames[4] = { "OpenGL", "Direct3D 11", "Direct3D 12", "Vulkan" };