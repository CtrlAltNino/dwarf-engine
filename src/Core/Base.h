#pragma once

#include <memory>
#include <array>
#include <string>

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
	// inline constexpr std::array<std::string, 4> graphicsApiNames = {"Direct3D 12", "Metal", "OpenGL", "Vulkan"};

	// A helper function to create a constexpr string
	constexpr std::string_view make_constexpr_string(const char *str)
	{
		return std::string_view(str);
	}

	// A helper function to create the graphicsApiNames array
	constexpr auto make_graphics_api_names()
	{
		return std::array<std::string_view, 4>{
			make_constexpr_string("Direct3D 12"),
			make_constexpr_string("Metal"),
			make_constexpr_string("OpenGL"),
			make_constexpr_string("Vulkan")};
	}

	inline constexpr auto graphicsApiNames = make_graphics_api_names();

	enum class ShaderParameterType
	{
		BOOLEAN,
		INTEGER,
		UNSIGNED_INTEGER,
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