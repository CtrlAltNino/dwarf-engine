#pragma once

#include <memory>

namespace Dwarf
{

	template <typename T>
	using Scope = std::unique_ptr<T>;
	template <typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args &&...args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args &&...args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	/// @brief Enum containing the different API's
	enum class GraphicsApi
	{
		D3D12,
		Metal,
		OpenGL,
		Vulkan
	};

	/// @brief Array containing the API names.
	static const char *graphicsApiNames[4] = {"Direct3D 12", "Metal", "OpenGL", "Vulkan"};

	enum ShaderParameterType
	{
		BOOLEAN,
		INTEGER,
		FLOAT,
		TEX2D,
		VEC2,
		VEC3,
		VEC4
	};

	static const char *parameterTypeNames[7] = {"Boolean", "Integer", "Float", "2D Texture", "Vector2", "Vector3", "Vector4"};

	static const char *reservedParameterNames[7] = {"_Time", "modelMatrix", "viewMatrix", "projectionMatrix", "fogStart", "fogEnd", "fogColor"};

#ifdef WIN32
	static bool apiAvailability[4] = {
		true,
		false,
		true,
		true,
	};
#elif __linux__
	static bool apiAvailability[4] = {
		false,
		false,
		true,
		true,
	};
#elif __APPLE__
	static bool apiAvailability[4] = {
		false,
		true,
		false,
		false,
	};
#endif

}