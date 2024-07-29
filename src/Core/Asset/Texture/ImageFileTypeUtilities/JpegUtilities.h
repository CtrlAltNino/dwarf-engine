#pragma once

#include "Utilities/ImageUtilities/TextureCommon.h"
#include <turbojpeg.h>
namespace Dwarf
{
  class JpegUtilities
  {
  public:
    static std::shared_ptr<TextureContainer>
    LoadJpeg(std::filesystem::path const& path)
    {
      tjhandle jpegDecompressor = tjInitDecompress();
      if (!jpegDecompressor)
      {
        // DWARF_CORE_ERROR("Failed to initialize JPEG decompressor");
        return nullptr;
      }

      std::ifstream file(path.string().c_str(), std::ios::binary);
      if (!file)
      {
        // DWARF_CORE_ERROR("Failed to open file: {0}", path.string());
        tjDestroy(jpegDecompressor);
        return nullptr;
      }

      std::vector<unsigned char> jpegData(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
      file.close();

      int width, height;
      tjDecompressHeader2(jpegDecompressor,
                          jpegData.data(),
                          jpegData.size(),
                          &width,
                          &height,
                          nullptr);

      unsigned char* imageData = new unsigned char[width * height * 3];
      tjDecompress2(jpegDecompressor,
                    jpegData.data(),
                    jpegData.size(),
                    imageData,
                    width,
                    0,
                    height,
                    TJPF_RGB,
                    TJFLAG_FASTDCT);

      tjDestroy(jpegDecompressor);

      std::shared_ptr<TextureContainer> textureData =
        std::make_shared<TextureContainer>();
      textureData->Size = glm::ivec2(width, height);
      textureData->Format = TextureFormat::RGB;
      textureData->Type = TextureType::TEXTURE_2D;
      textureData->DataType = TextureDataType::UNSIGNED_BYTE;
      textureData->ImageData = imageData;

      return textureData;
    }
  };
}