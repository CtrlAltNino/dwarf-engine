#include "ImageFileLoader.h"

#include "Core/Asset/Texture/ImageFileTypeUtilities/BmpUtilities.h"
#include "Core/Asset/Texture/ImageFileTypeUtilities/HdrUtilities.h"
#include "Core/Asset/Texture/ImageFileTypeUtilities/JpegUtilities.h"
#include "Core/Asset/Texture/ImageFileTypeUtilities/PngUtilities.h"
#include "Core/Asset/Texture/ImageFileTypeUtilities/TgaUtilities.h"
#include "Core/Asset/Texture/ImageFileTypeUtilities/TiffUtilities.h"

namespace Dwarf
{
  ImageFileLoader::ImageFileLoader(std::shared_ptr<IDwarfLogger> logger)
    : m_Logger(logger)
  {
    m_Logger->LogInfo(Log("ImageFileLoader created", "ImageFileLoader"));
  }

  ImageFileLoader::~ImageFileLoader()
  {
    m_Logger->LogInfo(Log("ImageFileLoader destroyed", "ImageFileLoader"));
  }

  std::shared_ptr<TextureContainer>
  ImageFileLoader::LoadTexture(std::filesystem::path texturePath)
  {
    std::string                       ext = texturePath.extension().string();
    std::shared_ptr<TextureContainer> textureData = nullptr;

    m_Logger->LogInfo(
      Log("Loading image file: " + texturePath.string(), "ImageFileLoader"));

    if (ext == ".jpg" || ext == ".jpeg")
    {
      m_Logger->LogInfo(Log("Loading jpeg image file", "ImageFileLoader"));
      textureData = JpegUtilities::LoadJpeg(texturePath);
    }
    else if (ext == ".png")
    {
      m_Logger->LogInfo(Log("Loading png image file", "ImageFileLoader"));
      textureData = PngUtilities::LoadPng(texturePath);
    }
    else if (ext == ".bmp")
    {
      m_Logger->LogInfo(Log("Loading bmp image file", "ImageFileLoader"));
      textureData = BmpUtilities::LoadBmp(texturePath);
    }
    else if (ext == ".tga")
    {
      m_Logger->LogInfo(Log("Loading tga image file", "ImageFileLoader"));
      textureData = TgaUtilities::LoadTga(texturePath);
    }
    else if (ext == ".tif" || ext == ".tiff")
    {
      m_Logger->LogInfo(Log("Loading tiff image file", "ImageFileLoader"));
      textureData = TiffUtilities::LoadTiff(texturePath);
    }
    else if (ext == ".hdr" || ext == ".exr")
    {
      m_Logger->LogInfo(Log("Loading hdr image file", "ImageFileLoader"));
      textureData = HdrUtilities::LoadHdr(texturePath);
    }

    if (textureData == nullptr)
    {
      m_Logger->LogError(Log("Failed to load image file", "ImageFileLoader"));
    }
    else
    {
      m_Logger->LogInfo(Log("Image file loaded", "ImageFileLoader"));
    }

    return textureData;
  }
} // namespace Dwarf