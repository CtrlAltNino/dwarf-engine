#pragma once

#include "dpch.h"

enum class GraphicsApi {OpenGL, D3D11, D3D12, Vulkan};
static const char* graphicsApiNames[4] = { "OpenGL", "Direct3D 11", "Direct3D 12", "Vulkan" };

struct ProjectData {
	std::string name;
	std::string path;
	GraphicsApi graphicsApi;
};