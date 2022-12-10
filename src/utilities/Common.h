#pragma once

#include "dpch.h"

/// @brief Enum containing the different API's
enum class GraphicsApi {OpenGL, D3D11, D3D12, Vulkan};

/// @brief Array containing the API names.
static const char* graphicsApiNames[4] = { "OpenGL", "Direct3D 11", "Direct3D 12", "Vulkan" };

/// @brief Structure to contain project data.
struct ProjectData {
	std::string name;
	std::string path;
	GraphicsApi graphicsApi;
};