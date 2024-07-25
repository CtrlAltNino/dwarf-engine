#include "TextureLoader.h"

#include "Core/Asset/Texture/ImageFileLoaders/BmpUtilities.h"
#include "Core/Asset/Texture/ImageFileLoaders/HdrUtilities.h"
#include "Core/Asset/Texture/ImageFileLoaders/JpegUtilities.h"
#include "Core/Asset/Texture/ImageFileLoaders/PngUtilities.h"
#include "Core/Asset/Texture/ImageFileLoaders/TgaUtilities.h"
#include "Core/Asset/Texture/ImageFileLoaders/TiffUtilities.h"

namespace Dwarf
{
  std::shared_ptr<TextureContainer>
  TextureLoader::LoadTexture(std::filesystem::path texturePath)
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