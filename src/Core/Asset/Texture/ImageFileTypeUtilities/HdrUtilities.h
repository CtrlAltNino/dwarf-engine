#pragma once

#include "Utilities/ImageUtilities/TextureCommon.h"
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
      // int      width;
      // int      height;
      // int      channels;
      // stbi_uc* data =
      //   stbi_load(path.string().c_str(), &width, &height, &channels, 0);
      // // ImageData<unsigned char> imageData;
      // // imageData.Data =
      // //     std::vector<unsigned char>(data, data + width * height *
      // channels);

      // std::shared_ptr<TextureContainer> textureData =
      // std::make_shared<TextureContainer>(); textureData->Width = width;
      // textureData->Height = height;
      // textureData->Format = TextureFormat::RGBA;
      // textureData->Type = TextureType::TEXTURE_2D;
      // textureData->DataType = TextureDataType::UNSIGNED_BYTE;

      // // copy content from data into textureData->ImageData
      // textureData->ImageData = data;

      // return Texture::Create(parameters, textureData);
      return nullptr;
    }
  };
}