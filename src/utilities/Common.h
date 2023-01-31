#pragma once

#include "dpch.h"

/// @brief Enum containing the different API's
enum class GraphicsApi { D3D11, D3D12, Metal, OpenGL, Vulkan };

/// @brief Array containing the API names.
static const char* graphicsApiNames[5] = { "Direct3D 11", "Direct3D 12", "Metal", "OpenGL", "Vulkan" };

#ifdef __WIN32__
	static bool apiAvailability[5] = { true, true, true, false, true };
#elif __APPLE__
	static bool apiAvailability[5] = { false, false, false, true, false };
#elif __linux__
	static bool apiAvailability[5] = { false, false, true, false, true };
#endif

/// @brief Structure to contain project data.
struct ProjectData {
	std::string name;
	std::string path;
	GraphicsApi graphicsApi;
};