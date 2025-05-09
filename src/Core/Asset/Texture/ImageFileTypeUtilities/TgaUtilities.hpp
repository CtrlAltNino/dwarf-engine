#pragma once

#include "Logging/IDwarfLogger.hpp"
#include "Utilities/FileHandler/IFileHandler.hpp"
#include "Utilities/ImageUtilities/TextureCommon.hpp"
#include <FreeImage.h>
#include <cstddef>
#include <filesystem>
#include <memory>

namespace Dwarf
{
  class TgaUtilities
  {
  public:
    static auto
    LoadTga(const std::shared_ptr<IDwarfLogger>& logger,
            const std::shared_ptr<IFileHandler>& fileHandler,
            const std::filesystem::path&         path,
            nlohmann::json& metadata) -> std::shared_ptr<TextureContainer>
    {
      // Ensure the file exists
      if (!fileHandler->FileExists(path))
      {
        logger->LogError(
          Log(fmt::format("File does not exist: {}", path.string()),
              "TgaUtilities"));
        return nullptr;
      }

      // Initialize FreeImage
      FreeImage_Initialise();

      // Load the image
      FIBITMAP* bitmap = FreeImage_Load(FIF_TARGA, path.string().c_str());
      if (bitmap == nullptr)
      {
        logger->LogError(
          Log(fmt::format("Failed to load TGA file: {}", path.string()),
              "TgaUtilities"));
        FreeImage_DeInitialise();
        return nullptr;
      }

      // Get image dimensions and format
      uint32_t width = FreeImage_GetWidth(bitmap);
      uint32_t height = FreeImage_GetHeight(bitmap);
      uint32_t channels = FreeImage_GetBPP(bitmap) / 8;

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
      std::vector<uint8_t> imageData(
        data, data + (static_cast<size_t>(width * height * channels)));

      if (metadata.contains("FlipY") && metadata["FlipY"].get<bool>())
      {
        for (int i = 1; i < imageData.size(); i += channels)
        {
          imageData[i] = 254 - imageData[i];
        }
      }

      // Swap the channels from BGRA to RGBA
      if (channels == 4)
      {
        for (uint32_t i = 0; i < width * height * channels; i += channels)
        {
          std::swap(imageData[i], imageData[i + 2]); // Swap B and R
        }
      }
      else if (channels == 3)
      {
        for (uint32_t i = 0; i < width * height * channels; i += channels)
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