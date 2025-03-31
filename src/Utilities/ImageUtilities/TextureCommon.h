#pragma once

#include "Utilities/ISerializable.h"
#include <cmath>
#include <cstdint>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
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
    bool             MipMapped = false;
    bool             IsSRGB = false;
    bool             FlipY = false;
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
                 std::vector<uint32_t>,
                 std::vector<float>>
             ImageData;
    uint32_t Samples = 1;
  };

  enum TextureFileType
  {
    Default,
    NormalMap
  };

  enum ColorSpace
  {
    Linear,
    Srgb
  };

  enum FilterMode
  {
    Nearest,
    Bilinear
  };

  enum WrapMode
  {
    Clamp,
    Mirror,
    Repeat
  };

  inline auto
  CalculateMipLevels(glm::ivec2 size) -> uint32_t
  {
    return 1 + (uint32_t)std::floor(std::log2(std::max(size.x, size.y)));
  }

  struct TextureImportSettings : public ISerializable
  {
    TextureFileType mTextureType = TextureFileType::Default;
    ColorSpace      mColorSpace = ColorSpace::Linear;
    bool            mGammaCorrection = false;
    bool            mGenerateMipMaps = false;
    bool            mFlipY = false;
    WrapMode        mWrapMode = WrapMode::Clamp;
    FilterMode      mFilterMode = FilterMode::Bilinear;

    TextureImportSettings() = default;

    TextureImportSettings(nlohmann::json& serializedData)
    {
      if (serializedData.contains("TextureType"))
      {
        mTextureType = serializedData["TextureType"].get<TextureFileType>();
      }

      if (serializedData.contains("ColorSpace"))
      {
        mColorSpace = serializedData["ColorSpace"].get<ColorSpace>();
      }

      if (serializedData.contains("GammaCorrection"))
      {
        mGammaCorrection = serializedData["GammaCorrection"].get<bool>();
      }

      if (serializedData.contains("GenerateMipMaps"))
      {
        mGenerateMipMaps = serializedData["GenerateMipMaps"].get<bool>();
      }

      if (serializedData.contains("FlipY"))
      {
        mFlipY = serializedData["FlipY"].get<bool>();
      }

      if (serializedData.contains("WrapMode"))
      {
        mWrapMode = serializedData["WrapMode"].get<WrapMode>();
      }

      if (serializedData.contains("FilterMode"))
      {
        mFilterMode = serializedData["FilterMode"].get<FilterMode>();
      }
    }

    nlohmann::json
    Serialize() override
    {
      nlohmann::json serializedData;

      serializedData["TextureType"] = mTextureType;

      serializedData["ColorSpace"] = mColorSpace;

      serializedData["GammaCorrection"] = mGammaCorrection;

      serializedData["GenerateMipMaps"] = mGenerateMipMaps;

      serializedData["FlipY"] = mFlipY;

      serializedData["WrapMode"] = mWrapMode;

      serializedData["FilterMode"] = mFilterMode;

      return serializedData;
    }
  };
} // namespace Dwarf