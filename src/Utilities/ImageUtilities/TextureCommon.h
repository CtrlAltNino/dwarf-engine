#pragma once

namespace Dwarf {
enum class TextureFormat { RED, RG, RGB, RGBA };

enum class TextureWrap {
  UNSET,
  REPEAT,
  MIRRORED_REPEAT,
  CLAMP_TO_EDGE,
  CLAMP_TO_BORDER
};

enum class TextureMinFilter {
  UNSET,
  NEAREST,
  LINEAR,
  NEAREST_MIPMAP_NEAREST,
  LINEAR_MIPMAP_NEAREST,
  NEAREST_MIPMAP_LINEAR,
  LINEAR_MIPMAP_LINEAR
};

enum class TextureMagFilter { UNSET, NEAREST, LINEAR };

enum class TextureType { TEXTURE_1D, TEXTURE_2D, TEXTURE_3D, TEXTURE_CUBE_MAP };

enum class TextureDataType {
  UNSIGNED_BYTE,
  FLOAT,
  UNSIGNED_INT,
  INT,
  UNSIGNED_SHORT
};

struct TextureContainer {
  TextureType Type = TextureType::TEXTURE_2D;
  TextureFormat Format = TextureFormat::RGBA;
  TextureDataType DataType = TextureDataType::UNSIGNED_BYTE;
  int Width = 0;
  int Height = 0;
  int Depth = 0;
  void *ImageData = nullptr;
};

struct TextureParameters {
  TextureWrap WrapS = TextureWrap::UNSET;
  TextureWrap WrapT = TextureWrap::UNSET;
  TextureWrap WrapR = TextureWrap::UNSET;
  TextureMinFilter MinFilter = TextureMinFilter::UNSET;
  TextureMagFilter MagFilter = TextureMagFilter::UNSET;
};
} // namespace Dwarf