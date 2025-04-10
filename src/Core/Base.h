#pragma once

#include <array>
#include <string>

namespace Dwarf
{
#define GRAPHICS_API_NAME
  /// @brief Enum containing the different API's
  enum class GraphicsApi
  {
    None,
    D3D12,
    OpenGL,
    Vulkan
  };

  enum class ShaderParameterType
  {
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    INTEGER,
    IVEC2,
    IVEC3,
    IVEC4,
    UNSIGNED_INTEGER,
    UVEC2,
    UVEC3,
    UVEC4,
    BOOLEAN,
    MAT3,
    MAT4,
    TEX2D
  };

  // TODO: Replace by getting the enum names with magic_enum?
  inline const std::map<std::string, ShaderParameterType> parameterChoices = {
    { "Float", ShaderParameterType::FLOAT },
    { "Vector2", ShaderParameterType::VEC2 },
    { "Vector3", ShaderParameterType::VEC3 },
    { "Vector4", ShaderParameterType::VEC4 },
    { "Integer", ShaderParameterType::INTEGER },
    { "Int Vector2", ShaderParameterType::IVEC2 },
    { "Int Vector3", ShaderParameterType::IVEC3 },
    { "Int Vector4", ShaderParameterType::IVEC4 },
    { "Unsigned Integer", ShaderParameterType::UNSIGNED_INTEGER },
    { "Unsigned Int Vector2", ShaderParameterType::UVEC2 },
    { "Unsigned Int Vector3", ShaderParameterType::UVEC3 },
    { "Unsigned Int Vector4", ShaderParameterType::UVEC4 },
    { "Boolean", ShaderParameterType::BOOLEAN },
    { "2D Texture", ShaderParameterType::TEX2D }
  };

  inline const std::array<std::string, 8> reservedParameterNames = {
    "_Time",    "modelMatrix", "viewMatrix", "projectionMatrix",
    "fogStart", "fogEnd",      "fogColor",   "viewPosition"
  };

#ifdef _WIN32
  constexpr std::array<bool, 3> apiPlatformSupport = {
    true,
    true,
    true,
  };
#elif __linux__
  constexpr std::array<bool, 3> apiPlatformSupport = {
    false,
    true,
    true,
  };
#endif
  constexpr std::array<bool, 3> apiImplementationState = {
    false,
    true,
    false,
  };

  template<typename T>
  struct TypeIdentity
  {
    using type = T;
  };

  inline auto
  to_lower(const std::string& str) -> std::string
  {
    std::string result = str;
    std::ranges::transform(
      result, result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
  }
}