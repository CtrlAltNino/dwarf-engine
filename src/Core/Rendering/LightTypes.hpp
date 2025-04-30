#pragma once

namespace Dwarf
{
  /// @brief Enum representing light types.
  enum class LightType : uint8_t
  {
    Directional,
    PointLight,
    SpotLight
  };
}