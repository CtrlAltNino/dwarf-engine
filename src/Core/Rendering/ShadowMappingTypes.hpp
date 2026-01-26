#pragma once

namespace Dwarf
{
  enum class ShadowMapResolution : uint8_t
  {
    Low,    // 512
    Medium, // 1024
    High,   // 2048
    Ultra   // 4096
  };

  constexpr auto
  ToResolution(ShadowMapResolution resolution) -> uint32_t
  {
    switch (resolution)
    {
      case ShadowMapResolution::Low: return 512;
      case ShadowMapResolution::Medium: return 1024;
      case ShadowMapResolution::High: return 2048;
      case ShadowMapResolution::Ultra: return 4096;
    }
    // std::unreachable(); // C++23 (or assert(false))
    assert(false);
  }
}