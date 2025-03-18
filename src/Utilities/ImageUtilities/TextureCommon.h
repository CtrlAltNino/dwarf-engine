#pragma once

#include "Utilities/ISerializable.h"
#include <variant>
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

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

  struct TextureImportSettings : public ISerializable
  {
    TextureFileType m_TextureType = TextureFileType::Default;
    ColorSpace      m_ColorSpace = ColorSpace::Linear;
    bool            m_GammaCorrection = false;
    bool            m_GenerateMipMaps = false;
    bool            m_FlipY = false;
    WrapMode        m_WrapMode = WrapMode::Clamp;
    FilterMode      m_FilterMode = FilterMode::Bilinear;

    TextureImportSettings() = default;

    TextureImportSettings(nlohmann::json& serializedData)
    {
      if (serializedData.contains("TextureType"))
      {
        m_TextureType = serializedData["TextureType"].get<TextureFileType>();
      }

      if (serializedData.contains("ColorSpace"))
      {
        m_ColorSpace = serializedData["ColorSpace"].get<ColorSpace>();
      }

      if (serializedData.contains("GammaCorrection"))
      {
        m_GammaCorrection = serializedData["GammaCorrection"].get<bool>();
      }

      if (serializedData.contains("GenerateMipMaps"))
      {
        m_GenerateMipMaps = serializedData["GenerateMipMaps"].get<bool>();
      }

      if (serializedData.contains("FlipY"))
      {
        m_FlipY = serializedData["FlipY"].get<bool>();
      }

      if (serializedData.contains("WrapMode"))
      {
        m_WrapMode = serializedData["WrapMode"].get<WrapMode>();
      }

      if (serializedData.contains("FilterMode"))
      {
        m_FilterMode = serializedData["FilterMode"].get<FilterMode>();
      }
    }

    nlohmann::json
    Serialize() override
    {
      nlohmann::json serializedData;

      serializedData["TextureType"] = m_TextureType;

      serializedData["ColorSpace"] = m_ColorSpace;

      serializedData["GammaCorrection"] = m_GammaCorrection;

      serializedData["GenerateMipMaps"] = m_GenerateMipMaps;

      serializedData["FlipY"] = m_FlipY;

      serializedData["WrapMode"] = m_WrapMode;

      serializedData["FilterMode"] = m_FilterMode;

      return serializedData;
    }
  };
} // namespace Dwarf