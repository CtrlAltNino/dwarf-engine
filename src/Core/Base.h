#pragma once
#include <array>
#include <string>
#include <string_view>

namespace Dwarf
{
#define GRAPHICS_API_NAME
  /// @brief Enum containing the different API's
  enum class GraphicsApi
  {
    None,
    D3D12,
    Metal,
    OpenGL,
    Vulkan
  };

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

  // TODO: Replace by getting the enum names with magic_enum?
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