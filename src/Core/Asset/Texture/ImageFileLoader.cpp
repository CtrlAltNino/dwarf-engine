#include "ImageFileLoader.h"

#include "Core/Asset/Texture/ImageFileTypeUtilities/BmpUtilities.h"
#include "Core/Asset/Texture/ImageFileTypeUtilities/HdrUtilities.h"
#include "Core/Asset/Texture/ImageFileTypeUtilities/JpegUtilities.h"
#include "Core/Asset/Texture/ImageFileTypeUtilities/PngUtilities.h"
#include "Core/Asset/Texture/ImageFileTypeUtilities/TgaUtilities.h"
#include "Core/Asset/Texture/ImageFileTypeUtilities/TiffUtilities.h"
#include "Utilities/ImageUtilities/TextureCommon.h"

namespace Dwarf
{
  ImageFileLoader::ImageFileLoader(
    std::shared_ptr<IDwarfLogger>   logger,
    std::shared_ptr<IFileHandler>   fileHandler,
    std::shared_ptr<IAssetMetadata> assetMetadata)
    : m_Logger(logger)
    , m_FileHandler(fileHandler)
    , m_AssetMetadata(assetMetadata)
  {
    m_Logger->LogDebug(Log("ImageFileLoader created", "ImageFileLoader"));
  }

  ImageFileLoader::~ImageFileLoader()
  {
    m_Logger->LogDebug(Log("ImageFileLoader destroyed", "ImageFileLoader"));
  }

  std::shared_ptr<TextureContainer>
  ImageFileLoader::LoadImageFile(std::filesystem::path texturePath)
  {
    std::string                       ext = texturePath.extension().string();
    std::shared_ptr<TextureContainer> textureData = nullptr;
    nlohmann::json metadata = m_AssetMetadata->GetMetadata(texturePath);
    nlohmann::json importSettings = metadata.contains("ImportSettings")
                                      ? metadata["ImportSettings"]
                                      : nlohmann::json();

    m_Logger->LogDebug(
      Log("Loading image file: " + texturePath.string(), "ImageFileLoader"));

    if (ext == ".jpg" || ext == ".jpeg")
    {
      m_Logger->LogDebug(Log("Loading jpeg image file", "ImageFileLoader"));
      textureData = JpegUtilities::LoadJpeg(texturePath, importSettings);
    }
    else if (ext == ".png")
    {
      m_Logger->LogDebug(Log("Loading png image file", "ImageFileLoader"));
      textureData = PngUtilities::LoadPng(m_Logger, m_FileHandler, texturePath);
    }
    else if (ext == ".bmp")
    {
      m_Logger->LogDebug(Log("Loading bmp image file", "ImageFileLoader"));
      textureData = BmpUtilities::LoadBmp(texturePath);
    }
    else if (ext == ".tga")
    {
      m_Logger->LogDebug(Log("Loading tga image file", "ImageFileLoader"));
      textureData = TgaUtilities::LoadTga(texturePath);
    }
    else if (ext == ".tif" || ext == ".tiff")
    {
      m_Logger->LogDebug(Log("Loading tiff image file", "ImageFileLoader"));
      textureData = TiffUtilities::LoadTiff(texturePath);
    }
    else if (ext == ".hdr" || ext == ".exr")
    {
      m_Logger->LogDebug(Log("Loading hdr image file", "ImageFileLoader"));
      textureData = HdrUtilities::LoadHdr(texturePath);
    }

    if (textureData == nullptr)
    {
      m_Logger->LogError(Log("Failed to load image file", "ImageFileLoader"));
    }
    else
    {
      m_Logger->LogDebug(Log("Image file loaded", "ImageFileLoader"));
    }

    if (importSettings.contains("ColorSpace"))
    {
      if (importSettings["ColorSpace"].get<ColorSpace>() == ColorSpace::Srgb)
      {
        textureData->Parameters.IsSRGB = true;
      }
    }

    if (importSettings.contains("GenerateMipMaps"))
    {
      textureData->Parameters.MipMapped =
        importSettings["GenerateMipMaps"].get<bool>();
    }

    if (importSettings.contains("WrapMode"))
    {
      switch (importSettings["WrapMode"].get<WrapMode>())
      {
        using enum WrapMode;
        case Clamp:
          textureData->Parameters.WrapR = TextureWrap::CLAMP_TO_BORDER;
          textureData->Parameters.WrapS = TextureWrap::CLAMP_TO_BORDER;
          textureData->Parameters.WrapT = TextureWrap::CLAMP_TO_BORDER;
          break;
        case Mirror:
          textureData->Parameters.WrapR = TextureWrap::MIRRORED_REPEAT;
          textureData->Parameters.WrapS = TextureWrap::MIRRORED_REPEAT;
          textureData->Parameters.WrapT = TextureWrap::MIRRORED_REPEAT;
          break;
        case Repeat:
          textureData->Parameters.WrapR = TextureWrap::REPEAT;
          textureData->Parameters.WrapS = TextureWrap::REPEAT;
          textureData->Parameters.WrapT = TextureWrap::REPEAT;
          break;
      }
    }

    if (importSettings.contains("FilterMode"))
    {
      switch (importSettings["FilterMode"].get<FilterMode>())
      {
        using enum FilterMode;
        case Nearest:
          textureData->Parameters.MinFilter = TextureMinFilter::NEAREST;
          textureData->Parameters.MagFilter = TextureMagFilter::NEAREST;
          break;
        case Bilinear:
          textureData->Parameters.MinFilter = TextureMinFilter::LINEAR;
          textureData->Parameters.MagFilter = TextureMagFilter::LINEAR;
          break;
      }
    }

    return textureData;
  }
} // namespace Dwarf