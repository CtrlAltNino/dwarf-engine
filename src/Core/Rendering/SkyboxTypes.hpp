#pragma once

#include <cstdint>

namespace Dwarf
{
  enum class SkyboxSource : uint8_t
  {
    Color,
    Material,
    HDRI,
  };

  enum class CubemapResolutionEnum : uint8_t
  {
    Low,
    Medium,
    High,
    Ultra
  };

  static auto
  GetCubemapResolution(CubemapResolutionEnum resolutionEnum) -> uint32_t
  {
    uint32_t resolution;

    switch (resolutionEnum)
    {
      using enum CubemapResolutionEnum;
      case Low: resolution = 256; break;
      case Medium: resolution = 512; break;
      case High: resolution = 1024; break;
      case Ultra: resolution = 2048; break;
    }

    return resolution;
  }
}