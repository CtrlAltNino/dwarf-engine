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
  }
  std::shared_ptr<TextureContainer>
  ImageFileLoader::LoadTexture(std::filesystem::path texturePath)
  {
    std::string                       ext = texturePath.extension().string();
    std::shared_ptr<TextureContainer> textureData = nullptr;

    m_Logger->LogInfo(
      Log("Loading texture: " + texturePath.string(), "ImageFileLoader"));

    if (ext == ".jpg" || ext == ".jpeg")
    {
      m_Logger->LogInfo(Log("Loading jpeg texture", "ImageFileLoader"));
      textureData = JpegUtilities::LoadJpeg(texturePath);
    }
    else if (ext == ".png")
    {
      m_Logger->LogInfo(Log("Loading png texture", "ImageFileLoader"));
      textureData = PngUtilities::LoadPng(texturePath);
    }
    else if (ext == ".bmp")
    {
      m_Logger->LogInfo(Log("Loading bmp texture", "ImageFileLoader"));
      textureData = BmpUtilities::LoadBmp(texturePath);
    }
    else if (ext == ".tga")
    {
      m_Logger->LogInfo(Log("Loading tga texture", "ImageFileLoader"));
      textureData = TgaUtilities::LoadTga(texturePath);
    }
    else if (ext == ".tif" || ext == ".tiff")
    {
      m_Logger->LogInfo(Log("Loading tiff texture", "ImageFileLoader"));
      textureData = TiffUtilities::LoadTiff(texturePath);
    }
    else if (ext == ".hdr" || ext == ".exr")
    {
      m_Logger->LogInfo(Log("Loading hdr texture", "ImageFileLoader"));
      textureData = HdrUtilities::LoadHdr(texturePath);
    }

    return textureData;
  }
} // namespace Dwarf