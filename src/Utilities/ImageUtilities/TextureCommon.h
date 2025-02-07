#pragma once

#include <variant>
namespace Dwarf
{
  enum class TextureFormat
  {
    RED,
    RG,
    RGB,
    RGBA,
    DEPTH,
    STENCIL,
    DEPTH_STENCIL
  };

  enum class TextureWrap
  {
    REPEAT,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER
  };

  enum class TextureMinFilter
  {
    NEAREST,
    LINEAR,
    NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR
  };

  enum class TextureMagFilter
  {
    NEAREST,
    LINEAR
  };

  enum class TextureType
  {
    TEXTURE_1D,
    TEXTURE_2D,
    TEXTURE_3D,
    TEXTURE_CUBE_MAP
  };

  enum class TextureDataType
  {
    UNSIGNED_BYTE,
    UNSIGNED_SHORT,
    INT,
    UNSIGNED_INT,
    FLOAT
  };

  using TextureResolution = std::variant<glm::ivec1, glm::ivec2, glm::ivec3>;

  struct TextureParameters
  {
    TextureWrap      WrapS = TextureWrap::REPEAT;
    TextureWrap      WrapT = TextureWrap::REPEAT;
    TextureWrap      WrapR = TextureWrap::REPEAT;
    TextureMinFilter MinFilter = TextureMinFilter::LINEAR;
    TextureMagFilter MagFilter = TextureMagFilter::LINEAR;
  };

  struct TextureContainer
  {
    TextureType       Type = TextureType::TEXTURE_2D;
    TextureFormat     Format = TextureFormat::RGBA;
    TextureDataType   DataType = TextureDataType::UNSIGNED_BYTE;
    TextureResolution Size = glm::ivec2(0);
    TextureParameters Parameters;
    std::variant<std::vector<unsigned char>,
                 std::vector<unsigned short>,
                 std::vector<int>,
                 std::vector<unsigned int>,
                 std::vector<float>>
        ImageData;
    int Samples = 1;
  };
} // namespace Dwarf