#pragma once

#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <spng.h>
namespace Dwarf
{
  class PngUtilities
  {
  public:
    static std::shared_ptr<TextureContainer>
    LoadPng(std::shared_ptr<IDwarfLogger> logger,
            std::shared_ptr<IFileHandler> fileHandler,
            std::filesystem::path const&  path)
    {
      spng_ctx* png = spng_ctx_new(0);
      if (!png)
      {
        // DWARF_CORE_ERROR("Failed to initialize PNG decoder");
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
      if (result)
      {
        spng_ctx_free(png);
        return nullptr;
      }
      spng_set_chunk_limits(png, 1024 * 1024, 1024 * 1024);

      spng_ihdr ihdr;
      spng_get_ihdr(png, &ihdr);

      size_t pngSize = ihdr.width * ihdr.height * 4;

      // TODO: Add support for different PNG formats
      std::vector<unsigned char> imageData(pngSize);
      spng_decode_image(png, imageData.data(), pngSize, SPNG_FMT_RGBA8, 0);
      spng_ctx_free(png);

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