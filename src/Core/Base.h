#pragma once
#include <array>
#include <string>
#include <string_view>

namespace Dwarf
{
  /// @brief Enum containing the different API's
  enum class GraphicsApi
  {
    None,
    D3D12,
    Metal,
    OpenGL,
    Vulkan
  };

  /// @brief Array containing the API names.
  // inline constexpr std::array<std::string, 4> graphicsApiNames = {"Direct3D
  // 12", "Metal", "OpenGL", "Vulkan"};

  // A helper function to create a constexpr string
  constexpr auto
  makeConstexprString(const char* str) -> std::string_view
  {
    return std::string_view(str);
  }

  // A helper function to create the graphicsApiNames array
  constexpr auto
  makeGraphicsApiNames()
  {
    return std::array<std::string_view, 4>{ makeConstexprString("Direct3D 12"),
                                            makeConstexprString("Metal"),
                                            makeConstexprString("OpenGL"),
                                            makeConstexprString("Vulkan") };
  }

  inline constexpr auto graphicsApiNames = makeGraphicsApiNames();

#define GRAPHICS_API_STRING(api) std::string(graphicsApiNames[(int)(api) - 1])

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

  inline const std::array<std::string, 7> parameterTypeNames = {
    "Boolean", "Integer", "Float", "2D Texture", "Vector2", "Vector3", "Vector4"
  };

  inline const std::array<std::string, 8> reservedParameterNames = {
    "_Time",    "modelMatrix", "viewMatrix", "projectionMatrix",
    "fogStart", "fogEnd",      "fogColor",   "viewPosition"
  };

#ifdef _WIN32
  constexpr std::array<bool, 5> apiAvailability = {
    true,
    false,
    true,
    true,
  };
#elif __linux__
  constexpr std::array<bool, 5> apiAvailability = {
    false,
    false,
    true,
    true,
  };
#elif __APPLE__
  constexpr std::array<bool, 5> apiAvailability = {
    false,
    true,
    false,
    false,
  };
#endif

  template<typename T>
  struct TypeIdentity
  {
    using type = T;
  };
}