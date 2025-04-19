#pragma once

namespace Dwarf
{

  /**
   * @brief Enum class representing the type of an asset.
   */
  enum class ASSET_TYPE : uint8_t
  {
    UNKNOWN,
    MODEL,
    TEXTURE,
    SCENE,
    MATERIAL,
    VERTEX_SHADER,
    TESC_SHADER,
    TESE_SHADER,
    GEOMETRY_SHADER,
    FRAGMENT_SHADER,
    COMPUTE_SHADER,
    HLSL_SHADER
  };
}