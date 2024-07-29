#include "ImageFileLoader.h"

#include "Core/Asset/Texture/ImageFileTypeUtilities/BmpUtilities.h"
#include "Core/Asset/Texture/ImageFileTypeUtilities/HdrUtilities.h"
#include "Core/Asset/Texture/ImageFileTypeUtilities/JpegUtilities.h"
#include "Core/Asset/Texture/ImageFileTypeUtilities/PngUtilities.h"
#include "Core/Asset/Texture/ImageFileTypeUtilities/TgaUtilities.h"
#include "Core/Asset/Texture/ImageFileTypeUtilities/TiffUtilities.h"

namespace Dwarf
{
  std::shared_ptr<TextureContainer>
  ImageFileLoader::LoadTexture(std::filesystem::path texturePath)
  {
    std::string                       ext = texturePath.extension().string();
    std::shared_ptr<TextureContainer> textureData = nullptr;

    if (ext == ".jpg" || ext == ".jpeg")
      textureData = JpegUtilities::LoadJpeg(texturePath);
    else if (ext == ".png")
      textureData = PngUtilities::LoadPng(texturePath);
    else if (ext == ".bmp")
      textureData = BmpUtilities::LoadBmp(texturePath);
    else if (ext == ".tga")
      textureData = TgaUtilities::LoadTga(texturePath);
    else if (ext == ".tif" || ext == ".tiff")
      textureData = TiffUtilities::LoadTiff(texturePath);
    else if (ext == ".hdr" || ext == ".exr")
      textureData = HdrUtilities::LoadHdr(texturePath);

    return textureData;
  }
} // namespace Dwarf