#pragma once

#include "Logging/IDwarfLogger.hpp"
#include "Utilities/FileHandler/IFileHandler.hpp"
#include "Utilities/ImageUtilities/TextureCommon.hpp"
#include <cstddef>
#include <spng.h>

namespace Dwarf
{
  class PngUtilities
  {
  public:
    static auto
    LoadPng(const std::shared_ptr<IDwarfLogger>& logger,
            const std::shared_ptr<IFileHandler>& fileHandler,
            const std::filesystem::path&         path,
            nlohmann::json& metadata) -> std::shared_ptr<TextureContainer>
    {
      if (!fileHandler->FileExists(path))
      {
        logger->LogError(
          Log(fmt::format("File does not exist: {}", path.string()),
              "PngUtilities"));
      }

      spng_ctx* png = spng_ctx_new(0);
      if (png == nullptr)
      {
        spng_ctx_free(png);
        logger->LogError(
          Log("Failed to initialize PNG decoder", "PngUtilities"));
        return nullptr;
      }

      std::vector<unsigned char> pngData =
        fileHandler->ReadBinaryFileUnbuffered(path);

      if (pngData.empty())
      {
        spng_ctx_free(png);
        return nullptr;
      }

      int result = spng_set_png_buffer(png, pngData.data(), pngData.size());
      if (result != 0)
      {
        spng_ctx_free(png);
        return nullptr;
      }
      spng_set_chunk_limits(
        png, static_cast<long>(1024) * 1024, static_cast<long>(1024) * 1024);

      spng_ihdr ihdr{};
      spng_get_ihdr(png, &ihdr);

      size_t pngSize = static_cast<size_t>(ihdr.width * ihdr.height) * 4;

      // TODO: Add support for different PNG formats
      std::vector<unsigned char> imageData(pngSize);
      spng_decode_image(png, imageData.data(), pngSize, SPNG_FMT_RGBA8, 0);
      spng_ctx_free(png);

      if (metadata.contains("FlipY") && metadata["FlipY"].get<bool>())
      {
        for (int i = 1; i < imageData.size(); i += 4)
        {
          imageData[i] = 254 - imageData[i];
        }
      }

      std::shared_ptr<TextureContainer> textureData =
        std::make_shared<TextureContainer>();
      textureData->Size = glm::ivec2(ihdr.width, ihdr.height);
      // TODO: Add support for different PNG formats
      textureData->Format = TextureFormat::RGBA;
      textureData->Type = TextureType::TEXTURE_2D;
      textureData->DataType = ihdr.bit_depth == 8
                                ? TextureDataType::UNSIGNED_BYTE
                                : TextureDataType::UNSIGNED_SHORT;

      textureData->ImageData = imageData;
      return textureData;
    }
  };
}