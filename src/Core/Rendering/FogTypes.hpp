#pragma once

namespace Dwarf
{
  /// @brief Enum that represents the type of fog.
  enum class FogType : uint8_t
  {
    /// @brief Linear fog.
    Linear,
    /// @brief Exponential fog.
    Exponential
  };
}