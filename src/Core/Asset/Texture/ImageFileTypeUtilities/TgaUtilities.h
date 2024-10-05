#pragma once

#include "Utilities/ImageUtilities/TextureCommon.h"
#include <FreeImage.h>
#include <iostream>
#include <filesystem>
#include <memory>

namespace Dwarf
{
  class TgaUtilities
  {
  public:
    static std::shared_ptr<TextureContainer>
    LoadTga(const std::filesystem::path& path)
    {
      // Ensure the file exists
      if (!std::filesystem::exists(path))
      {
        std::cerr << "File does not exist: " << path << std::endl;
        return nullptr;
      }

      // Initialize FreeImage
      FreeImage_Initialise();

      // Load the image
      FIBITMAP* bitmap = FreeImage_Load(FIF_TARGA, path.string().c_str());
      if (!bitmap)
      {
        std::cerr << "Failed to load TGA file: " << path << std::endl;
        FreeImage_DeInitialise();
        return nullptr;
      }

      // Get image dimensions and format
      int width = FreeImage_GetWidth(bitmap);
      int height = FreeImage_GetHeight(bitmap);
      int channels = FreeImage_GetBPP(bitmap) / 8;

      std::cout << "Loading TGA file: " << path << std::endl;
      std::cout << "Width: " << width << " Height: " << height
                << " Channels: " << channels << std::endl;

      // Convert the image to 32-bit (RGBA) if necessary
      FIBITMAP* convertedBitmap = bitmap;

      if (channels == 3)
      {
        convertedBitmap = FreeImage_ConvertTo24Bits(bitmap);
      }
      else if (channels == 4)
      {
        convertedBitmap = FreeImage_ConvertTo32Bits(bitmap);
      }

      FreeImage_Unload(bitmap);

      // Flip the image vertically
      FreeImage_FlipVertical(convertedBitmap);

      // Get the image data
      BYTE* data = FreeImage_GetBits(convertedBitmap);

      // Create the texture container
      std::shared_ptr<TextureContainer> textureData =
        std::make_shared<TextureContainer>();
      textureData->Size = glm::ivec2(width, height);
      textureData->Format =
        channels == 4 ? TextureFormat::RGBA : TextureFormat::RGB;
      textureData->Type = TextureType::TEXTURE_2D;
      textureData->DataType = TextureDataType::UNSIGNED_BYTE;

      // Copy content from data into textureData->ImageData
      std::vector<uint8_t> imageData(data, data + (width * height * channels));

      // Swap the channels from BGRA to RGBA
      if (channels == 4)
      {
        for (int i = 0; i < width * height * channels; i += channels)
        {
          std::swap(imageData[i], imageData[i + 2]); // Swap B and R
        }
      }
      else if (channels == 3)
      {
        for (int i = 0; i < width * height * channels; i += channels)
        {
          std::swap(imageData[i], imageData[i + 2]); // Swap B and R
        }
      }

      textureData->ImageData = std::move(imageData);

      // Unload the bitmap
      FreeImage_Unload(convertedBitmap);

      // Deinitialize FreeImage
      FreeImage_DeInitialise();

      return textureData;
    }
  };
}