#include "pch.hpp"

#include "Core/Asset/Texture/ImageFileTypeUtilities/BmpUtilities.hpp"
#include "Core/Asset/Texture/ImageFileTypeUtilities/HdrUtilities.hpp"
#include "Core/Asset/Texture/ImageFileTypeUtilities/JpegUtilities.hpp"
#include "Core/Asset/Texture/ImageFileTypeUtilities/PngUtilities.hpp"
#include "Core/Asset/Texture/ImageFileTypeUtilities/TgaUtilities.hpp"
#include "Core/Asset/Texture/ImageFileTypeUtilities/TiffUtilities.hpp"
#include "ImageFileLoader.hpp"
#include "Utilities/ImageUtilities/TextureCommon.hpp"
#include <cstdint>

namespace Dwarf
{
  ImageFileLoader::ImageFileLoader(
    std::shared_ptr<IDwarfLogger>   logger,
    std::shared_ptr<IFileHandler>   fileHandler,
    std::shared_ptr<IAssetMetadata> assetMetadata)
    : mLogger(std::move(logger))
    , mFileHandler(std::move(fileHandler))
    , mAssetMetadata(std::move(assetMetadata))
  {
    mLogger->LogDebug(Log("ImageFileLoader created", "ImageFileLoader"));
  }

  ImageFileLoader::~ImageFileLoader()
  {
    mLogger->LogDebug(Log("ImageFileLoader destroyed", "ImageFileLoader"));
  }

  auto
  ImageFileLoader::LoadImageFile(std::filesystem::path imagePath)
    -> std::shared_ptr<TextureContainer>
  {
    std::string                       ext = imagePath.extension().string();
    std::shared_ptr<TextureContainer> textureData = nullptr;
    nlohmann::json metadata = mAssetMetadata->GetMetadata(imagePath);
    nlohmann::json importSettings = metadata.contains("ImportSettings")
                                      ? metadata["ImportSettings"]
                                      : nlohmann::json();

    mLogger->LogDebug(
      Log("Loading image file: " + imagePath.string(), "ImageFileLoader"));

    if (ext == ".jpg" || ext == ".jpeg")
    {
      mLogger->LogDebug(Log("Loading jpeg image file", "ImageFileLoader"));
      textureData = JpegUtilities::LoadJpeg(
        mLogger, mFileHandler, imagePath, importSettings);
    }
    else if (ext == ".png")
    {
      mLogger->LogDebug(Log("Loading png image file", "ImageFileLoader"));
      textureData =
        PngUtilities::LoadPng(mLogger, mFileHandler, imagePath, importSettings);
    }
    else if (ext == ".bmp")
    {
      mLogger->LogDebug(Log("Loading bmp image file", "ImageFileLoader"));
      textureData = BmpUtilities::LoadBmp(imagePath);
    }
    else if (ext == ".tga")
    {
      mLogger->LogDebug(Log("Loading tga image file", "ImageFileLoader"));
      textureData =
        TgaUtilities::LoadTga(mLogger, mFileHandler, imagePath, importSettings);
    }
    else if (ext == ".tif" || ext == ".tiff")
    {
      mLogger->LogDebug(Log("Loading tiff image file", "ImageFileLoader"));
      textureData = TiffUtilities::LoadTiff(imagePath, importSettings);
    }
    else if (ext == ".hdr" || ext == ".exr")
    {
      mLogger->LogDebug(Log("Loading hdr image file", "ImageFileLoader"));
      textureData = HdrUtilities::LoadHdr(imagePath);
    }

    if (textureData == nullptr)
    {
      mLogger->LogError(Log("Failed to load image file", "ImageFileLoader"));
      return nullptr;
    }

    mLogger->LogDebug(Log("Image file loaded", "ImageFileLoader"));

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

    if (importSettings.contains("AnisoLevel"))
    {
      textureData->Parameters.AnisoLevel =
        importSettings["AnisoLevel"].get<uint8_t>();
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
          if (textureData->Parameters.MipMapped)
          {
            textureData->Parameters.MinFilter =
              TextureMinFilter::LINEAR_MIPMAP_LINEAR;
            textureData->Parameters.MagFilter = TextureMagFilter::LINEAR;
          }
          else
          {
            textureData->Parameters.MinFilter = TextureMinFilter::LINEAR;
            textureData->Parameters.MagFilter = TextureMagFilter::LINEAR;
          }
          break;
      }
    }

    return textureData;
  }
} // namespace Dwarf