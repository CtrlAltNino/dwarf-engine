#pragma once

#include <memory>
#include <array>

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
	inline constexpr std::array<std::string, 4> graphicsApiNames = {"Direct3D 12", "Metal", "OpenGL", "Vulkan"};

	enum class ShaderParameterType
	{
		BOOLEAN,
		INTEGER,
		FLOAT,
		TEX2D,
		VEC2,
		VEC3,
		VEC4
	};

	inline const std::array<std::string, 7> parameterTypeNames = {"Boolean", "Integer", "Float", "2D Texture", "Vector2", "Vector3", "Vector4"};

	inline const std::array<std::string, 7> reservedParameterNames = {"_Time", "modelMatrix", "viewMatrix", "projectionMatrix", "fogStart", "fogEnd", "fogColor"};

#ifdef WIN32
	constexpr std::array<bool, 4> apiAvailability = {
		true,
		false,
		true,
		true,
	};
#elif __linux__
	constexpr std::array<bool, 4> apiAvailability = {
		false,
		false,
		true,
		true,
	};
#elif __APPLE__
	constexpr std::array<bool, 4> apiAvailability = {
		false,
		true,
		false,
		false,
	};
#endif

}