#pragma once

namespace Dwarf
{
  enum class AntiAliasingMethod : uint8_t
  {
    None,
    FXAA,
    TAA,
    MSAA
  };
}