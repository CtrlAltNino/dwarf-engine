#pragma once

namespace Dwarf {

	/// @brief Enum containing the different API's
	enum class GraphicsApi { D3D11, D3D12, Metal, OpenGL, Vulkan };

	/// @brief Array containing the API names.
	static const char* graphicsApiNames[5] = { "Direct3D 11", "Direct3D 12", "Metal", "OpenGL", "Vulkan" };

	#ifdef WIN32
		static bool apiAvailability[5] = { true, true, false, true, true };
	#elif __APPLE__
		static bool apiAvailability[5] = { false, false, true, false, false };
	#elif __linux__
		static bool apiAvailability[5] = { false, false, false, true, true };
	#endif
}