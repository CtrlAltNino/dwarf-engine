#pragma once

#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <turbojpeg.h>

namespace Dwarf
{
  class JpegUtilities
  {
  public:
    static auto
    LoadJpeg(const std::shared_ptr<IDwarfLogger>& logger,
             const std::shared_ptr<IFileHandler>& fileHandler,
             std::filesystem::path const&         path,
             nlohmann::json& metadata) -> std::shared_ptr<TextureContainer>
    {
      if (!fileHandler->FileExists(path))
      {
        logger->LogError(
          Log(fmt::format("File does not exist: {}", path.string()),
              "JpegUtilities"));
        return nullptr;
      }

      tjhandle jpegDecompressor = tjInitDecompress();
      if (jpegDecompressor == nullptr)
      {
        logger->LogError(
          Log(fmt::format("Failed to initialize JPEG decompressor: {}",
                          tjGetErrorStr()),
              "JpegUtilities"));
        return nullptr;
      }

      std::vector<unsigned char> compressed(
        fileHandler->ReadBinaryFileUnbuffered(path));

      if (compressed.empty())
      {
        logger->LogError(
          Log(fmt::format("Failed to read JPEG file: {}", path.string()),
              "JpegUtilities"));
        tjDestroy(jpegDecompressor);
        return nullptr;
      }

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
        logger->LogError(
          Log(fmt::format("Failed to read JPEG header: {}", tjGetErrorStr()),
              "JpegUtilities"));
        tjDestroy(jpegDecompressor);
        return nullptr;
      }

      auto* decompressed = new unsigned char[width * height * 3];
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
        logger->LogError(Log(
          fmt::format("Failed to decompress JPEG image: {}", tjGetErrorStr()),
          "JpegUtilities"));
        tjFree(decompressed);
        tjDestroy(jpegDecompressor);
        return nullptr;
      }

      tjDestroy(jpegDecompressor);

      auto dataVec = std::vector<unsigned char>(
        decompressed, decompressed + (width * height * 3));

      if (metadata.contains("FlipY") && metadata["FlipY"].get<bool>())
      {
        for (int i = 1; i < dataVec.size(); i += 3)
        {
          dataVec[i] = 254 - dataVec[i];
        }
      }

      std::shared_ptr<TextureContainer> textureData =
        std::make_shared<TextureContainer>();
      textureData->Size = glm::ivec2(width, height);
      textureData->Format = TextureFormat::RGB;
      textureData->Type = TextureType::TEXTURE_2D;
      textureData->DataType = TextureDataType::UNSIGNED_BYTE;
      textureData->ImageData = dataVec;

      return textureData;
    }
  };
}