#pragma once

#include "Utilities/ImageUtilities/TextureCommon.h"
#include <turbojpeg.h>
namespace Dwarf
{
  class JpegUtilities
  {
  public:
    static auto
    LoadJpeg(std::filesystem::path const& path, nlohmann::json& metadata)
      -> std::shared_ptr<TextureContainer>
    {
      tjhandle jpegDecompressor = tjInitDecompress();
      if (jpegDecompressor == nullptr)
      {
        std::cerr << "Failed to initialize JPEG decompressor: "
                  << tjGetErrorStr() << '\n';
        return nullptr;
      }

      std::ifstream file(path.string().c_str(), std::ios::binary);
      if (!file)
      {
        std::cerr << "Failed to open file: " << path.string() << '\n';
        tjDestroy(jpegDecompressor);
        return nullptr;
      }

      std::vector<unsigned char> compressed(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

      if (compressed.empty())
      {
        std::cerr << "Failed to read JPEG file: " << path.string() << '\n';
        tjDestroy(jpegDecompressor);
        return nullptr;
      }

      // Check file size
      std::cerr << "JPEG file size: " << compressed.size() << " bytes" << '\n';

      // Print first few bytes of the JPEG data
      std::cerr << "First few bytes of JPEG data: ";
      for (size_t i = 0; i < std::min(compressed.size(), size_t(10)); ++i)
      {
        std::cerr << std::hex << static_cast<int>(compressed[i]) << " ";
      }
      std::cerr << std::dec << '\n';

      int jpegSubsamp = 0;
      int width = 0;
      int height = 0;
      if (tjDecompressHeader2(jpegDecompressor,
                              compressed.data(),
                              compressed.size(),
                              &width,
                              &height,
                              &jpegSubsamp) == -1)
      {
        std::cerr << "Failed to read JPEG header: " << tjGetErrorStr() << '\n';
        tjDestroy(jpegDecompressor);
        return nullptr;
      }

      unsigned char* decompressed = new unsigned char[width * height * 3];
      if (tjDecompress2(jpegDecompressor,
                        compressed.data(),
                        compressed.size(),
                        decompressed,
                        width,
                        0,
                        height,
                        TJPF_RGB,
                        TJFLAG_FASTDCT) == -1)
      {
        std::cerr << "Failed to decompress JPEG image: " << tjGetErrorStr()
                  << '\n';
        tjFree(decompressed);
        tjDestroy(jpegDecompressor);
        return nullptr;
      }

      tjDestroy(jpegDecompressor);

      std::shared_ptr<TextureContainer> textureData =
        std::make_shared<TextureContainer>();
      textureData->Size = glm::ivec2(width, height);
      textureData->Format = TextureFormat::RGB;
      textureData->Type = TextureType::TEXTURE_2D;
      textureData->DataType = TextureDataType::UNSIGNED_BYTE;
      textureData->ImageData = std::vector<unsigned char>(
        decompressed, decompressed + (width * height * 3));

      file.close();
      return textureData;
    }
  };
}