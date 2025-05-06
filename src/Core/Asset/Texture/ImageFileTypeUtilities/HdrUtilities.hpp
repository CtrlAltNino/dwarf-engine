#pragma once

#include "Utilities/ImageUtilities/TextureCommon.hpp"
#include <FreeImage.h>
#include <glm/fwd.hpp>
#include <stb_image.h>


namespace Dwarf
{
  class HdrUtilities
  {
  public:
    static auto
    LoadHdr(std::filesystem::path const& path)
      -> std::shared_ptr<TextureContainer>
    {
      static bool freeImageInitialized = false;
      if (!freeImageInitialized)
      {
        FreeImage_Initialise();
        freeImageInitialized = true;
      }

      FREE_IMAGE_FORMAT format =
        FreeImage_GetFileType(path.string().c_str(), 0);
      if (format == FIF_UNKNOWN)
      {
        format = FreeImage_GetFIFFromFilename(path.string().c_str());
      }

      if (format == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(format))
      {
        std::cerr << "FreeImage cannot read file format: " << path << "\n";
        return nullptr;
      }

      FIBITMAP* bitmap =
        FreeImage_Load(format, path.string().c_str(), HDR_DEFAULT);
      if (!bitmap)
      {
        std::cerr << "FreeImage failed to load HDR image: " << path << "\n";
        return nullptr;
      }

      FIBITMAP* floatImage = FreeImage_ConvertToRGBF(bitmap);
      FreeImage_Unload(bitmap);

      if (!floatImage)
      {
        std::cerr << "Failed to convert image to 32-bit float RGB: " << path
                  << "\n";
        return nullptr;
      }

      int      width = FreeImage_GetWidth(floatImage);
      int      height = FreeImage_GetHeight(floatImage);
      unsigned pitch = FreeImage_GetPitch(floatImage);
      BYTE*    bits = FreeImage_GetBits(floatImage);

      std::vector<float> dataVec;
      dataVec.resize(width * height * 3); // RGB

      // FreeImage stores image data bottom-up
      for (int y = 0; y < height; ++y)
      {
        const float* scanline =
          reinterpret_cast<const float*>(bits + y * pitch);
        for (int x = 0; x < width; ++x)
        {
          int dstIdx = ((height - 1 - y) * width + x) * 3;
          dataVec[dstIdx + 0] = scanline[x * 3 + 2]; // R (FreeImage is BGR)
          dataVec[dstIdx + 1] = scanline[x * 3 + 1]; // G
          dataVec[dstIdx + 2] = scanline[x * 3 + 0]; // B
        }
      }

      FreeImage_Unload(floatImage);

      auto textureData = std::make_shared<TextureContainer>();
      textureData->Size = glm::ivec2(width, height);
      textureData->Format = TextureFormat::RGB;
      textureData->Type = TextureType::TEXTURE_2D;
      textureData->DataType = TextureDataType::FLOAT;
      textureData->ImageData = std::move(dataVec);

      return textureData;
    }
  };
}