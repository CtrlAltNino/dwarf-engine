#pragma once

#include "Utilities/ImageUtilities/TextureCommon.h"
#include <stb_image.h>

namespace Dwarf
{
  class TiffUtilities
  {
  public:
    static auto
    LoadTiff(std::filesystem::path const& path, nlohmann::json& metadata)
      -> std::shared_ptr<TextureContainer>
    {
      // TODO: Remove stbi
      int      width = 0;
      int      height = 0;
      int      channels = 0;
      stbi_uc* data =
        stbi_load(path.string().c_str(), &width, &height, &channels, 0);

      std::shared_ptr<TextureContainer> textureData =
        std::make_shared<TextureContainer>();
      textureData->Size = glm::ivec2(width, height);
      textureData->Format =
        channels == 4 ? TextureFormat::RGBA : TextureFormat::RGB;
      textureData->Type = TextureType::TEXTURE_2D;
      textureData->DataType = TextureDataType::UNSIGNED_BYTE;

      auto dataVec =
        std::vector<unsigned char>(data, data + (width * height * channels));

      if (metadata.contains("FlipY") && metadata["FlipY"].get<bool>())
      {
        for (int i = 1; i < dataVec.size(); i += channels)
        {
          dataVec[i] = 254 - dataVec[i];
        }
      }

      // copy content from data into textureData->ImageData
      textureData->ImageData = dataVec;

      return textureData;
    }
  };
}