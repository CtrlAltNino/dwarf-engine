#include "Core/Rendering/Framebuffer.h"
#include "Utilities/FileHandler.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include "dpch.h"
#include <iostream>
#include <spng.h>
#include <turbojpeg.h>

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Core/Rendering/TextureCreator.h"

namespace Dwarf
{
  std::shared_ptr<Texture>
  TextureCreator::FromPath(std::filesystem::path const& path)
  {
    std::string                        ext = path.extension().string();
    std::shared_ptr<TextureParameters> parameters = GetParameters(path);

    if (ext == ".jpg" || ext == ".jpeg")
      return FromJpeg(path, parameters);
    else if (ext == ".png")
      return FromPng(path, parameters);
    else if (ext == ".bmp")
      return FromBmp(path, parameters);
    else if (ext == ".tga")
      return FromTga(path, parameters);
    else if (ext == ".hdr" || ext == ".exr")
      return FromHdr(path, parameters);
    else if (ext == ".tiff" || ext == ".tif")
      return FromTiff(path, parameters);
    else
      return nullptr;
  }

  std::shared_ptr<TextureParameters>
  TextureCreator::GetParameters(std::filesystem::path const& path)
  {
    std::shared_ptr<TextureParameters> parameters =
      std::make_shared<TextureParameters>();
    parameters->WrapR = TextureWrap::REPEAT;
    parameters->WrapS = TextureWrap::REPEAT;
    parameters->WrapT = TextureWrap::REPEAT;
    parameters->MinFilter = TextureMinFilter::LINEAR;
    parameters->MagFilter = TextureMagFilter::LINEAR;
    return parameters;
  }

  std::shared_ptr<Texture>
  TextureCreator::FromPng(std::filesystem::path const&       path,
                          std::shared_ptr<TextureParameters> parameters)
  {
    spng_ctx* png = spng_ctx_new(0);
    if (!png)
    {
      // DWARF_CORE_ERROR("Failed to initialize PNG decoder");
      return nullptr;
    }

    FILE* file = fopen(path.string().c_str(), "rb");
    if (!file)
    {
      spng_ctx_free(png);
      return nullptr;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char* pngData = new unsigned char[fileSize];
    fread(pngData, 1, fileSize, file);
    fclose(file);

    spng_set_png_buffer(png, pngData, fileSize);
    spng_set_chunk_limits(png, 1024 * 1024, 1024 * 1024);

    spng_ihdr ihdr;
    spng_get_ihdr(png, &ihdr);

    // TODO: Add support for different PNG formats
    unsigned char* imageData = new unsigned char[ihdr.width * ihdr.height * 4];
    spng_decode_image(
      png, imageData, ihdr.width * ihdr.height * 4, SPNG_FMT_RGBA8, 0);

    std::shared_ptr<TextureContainer> textureData =
      std::make_shared<TextureContainer>();
    textureData->Width = ihdr.width;
    textureData->Height = ihdr.height;
    // TODO: Add support for different PNG formats
    textureData->Format = TextureFormat::RGBA;
    textureData->Type = TextureType::TEXTURE_2D;
    textureData->DataType = ihdr.bit_depth == 8
                              ? TextureDataType::UNSIGNED_BYTE
                              : TextureDataType::UNSIGNED_SHORT;

    // copy content from data into textureData->ImageData
    textureData->ImageData = imageData;

    std::shared_ptr<Texture> texture = Texture::Create(textureData, parameters);

    spng_ctx_free(png);

    return texture;
  }

  std::shared_ptr<Texture>
  TextureCreator::FromJpeg(std::filesystem::path const&       path,
                           std::shared_ptr<TextureParameters> parameters)
  {
    tjhandle jpegDecompressor = tjInitDecompress();
    if (!jpegDecompressor)
    {
      // DWARF_CORE_ERROR("Failed to initialize JPEG decompressor");
      return nullptr;
    }

    FILE* file = fopen(path.string().c_str(), "rb");
    if (!file)
    {
      // DWARF_CORE_ERROR("Failed to open file: {0}", path.string());
      tjDestroy(jpegDecompressor);
      return nullptr;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char* jpegData = new unsigned char[fileSize];
    fread(jpegData, 1, fileSize, file);
    fclose(file);

    int width, height;
    tjDecompressHeader2(
      jpegDecompressor, jpegData, fileSize, &width, &height, nullptr);

    unsigned char* imageData = new unsigned char[width * height * 3];
    tjDecompress2(jpegDecompressor,
                  jpegData,
                  fileSize,
                  imageData,
                  width,
                  0,
                  height,
                  TJPF_RGB,
                  TJFLAG_FASTDCT);

    tjDestroy(jpegDecompressor);

    std::shared_ptr<TextureContainer> data =
      std::make_shared<TextureContainer>();
    data->Width = width;
    data->Height = height;
    data->Format = TextureFormat::RGB;
    data->Type = TextureType::TEXTURE_2D;
    data->DataType = TextureDataType::UNSIGNED_BYTE;
    data->ImageData = imageData;

    return Texture::Create(data, parameters);
  }

  std::shared_ptr<Texture>
  TextureCreator::FromTga(std::filesystem::path const&       path,
                          std::shared_ptr<TextureParameters> parameters)
  {
    // TODO: Remove stbi
    int      width;
    int      height;
    int      channels;
    stbi_uc* data =
      stbi_load(path.string().c_str(), &width, &height, &channels, 0);

    std::shared_ptr<TextureContainer> textureData =
      std::make_shared<TextureContainer>();
    textureData->Width = width;
    textureData->Height = height;
    textureData->Format =
      channels == 4 ? TextureFormat::RGBA : TextureFormat::RGB;
    textureData->Type = TextureType::TEXTURE_2D;
    textureData->DataType = TextureDataType::UNSIGNED_BYTE;

    // copy content from data into textureData->ImageData
    textureData->ImageData = data;

    return Texture::Create(textureData, parameters);
  }

  std::shared_ptr<Texture>
  TextureCreator::FromHdr(std::filesystem::path const&       path,
                          std::shared_ptr<TextureParameters> parameters)
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

  std::shared_ptr<Texture>
  TextureCreator::FromTiff(std::filesystem::path const&       path,
                           std::shared_ptr<TextureParameters> parameters)
  {
    // TODO: Remove stbi
    int      width;
    int      height;
    int      channels;
    stbi_uc* data =
      stbi_load(path.string().c_str(), &width, &height, &channels, 0);

    std::shared_ptr<TextureContainer> textureData =
      std::make_shared<TextureContainer>();
    textureData->Width = width;
    textureData->Height = height;
    textureData->Format =
      channels == 4 ? TextureFormat::RGBA : TextureFormat::RGB;
    textureData->Type = TextureType::TEXTURE_2D;
    textureData->DataType = TextureDataType::UNSIGNED_BYTE;

    // copy content from data into textureData->ImageData
    textureData->ImageData = data;

    return Texture::Create(textureData, parameters);
  }

  std::shared_ptr<Texture>
  TextureCreator::FromBmp(std::filesystem::path const&       path,
                          std::shared_ptr<TextureParameters> parameters)
  {
    // TODO: Remove stbi
    int      width;
    int      height;
    int      channels;
    stbi_uc* data =
      stbi_load(path.string().c_str(), &width, &height, &channels, 0);

    std::shared_ptr<TextureContainer> textureData =
      std::make_shared<TextureContainer>();
    textureData->Width = width;
    textureData->Height = height;
    textureData->Format =
      channels == 4 ? TextureFormat::RGBA : TextureFormat::RGB;
    textureData->Type = TextureType::TEXTURE_2D;
    textureData->DataType = TextureDataType::UNSIGNED_BYTE;

    // copy content from data into textureData->ImageData
    textureData->ImageData = data;

    return Texture::Create(textureData, parameters);
  }

  std::shared_ptr<Texture>
  TextureCreator::FromData(TextureParameters const& parameters,
                           glm::ivec2               size,
                           void*                    data)
  {
    std::shared_ptr<TextureContainer> textureData =
      std::make_shared<TextureContainer>();
    textureData->Width = size.x;
    textureData->Height = size.y;
    textureData->Format = TextureFormat::RGBA;
    textureData->Type = TextureType::TEXTURE_2D;
    textureData->DataType = TextureDataType::UNSIGNED_BYTE;
    textureData->ImageData = (unsigned char*)data;

    return Texture::Create(textureData,
                           std::make_shared<TextureParameters>(parameters));
  }

  std::shared_ptr<Texture>
  TextureCreator::Empty(glm::ivec2 size)
  {
    std::shared_ptr<TextureContainer> textureData =
      std::make_shared<TextureContainer>();
    textureData->Width = size.x;
    textureData->Height = size.y;
    textureData->Format = TextureFormat::RGBA;
    textureData->Type = TextureType::TEXTURE_2D;
    textureData->DataType = TextureDataType::UNSIGNED_BYTE;
    textureData->ImageData = nullptr;

    return Texture::Create(textureData);
  }

  std::shared_ptr<Texture>
  TextureCreator::Empty(FramebufferTextureSpecification const& parameters,
                        glm::ivec2                             size,
                        int                                    samples)
  {
    std::shared_ptr<TextureContainer> textureData =
      std::make_shared<TextureContainer>();
    textureData->Width = size.x;
    textureData->Height = size.y;
    textureData->Format = TextureFormat::RGBA;
    textureData->Type = TextureType::TEXTURE_2D;
    textureData->DataType = TextureDataType::UNSIGNED_BYTE;
    textureData->ImageData = nullptr;
    textureData->Samples = samples;

    switch (parameters.TextureFormat)
    {
      case FramebufferTextureFormat::RGBA8:
        textureData->Format = TextureFormat::RGBA;
        break;
      case FramebufferTextureFormat::RED_INTEGER:
        textureData->Format = TextureFormat::RED;
        textureData->DataType = TextureDataType::UNSIGNED_INT;
        break;
      case FramebufferTextureFormat::DEPTH24STENCIL8:
        textureData->Format = TextureFormat::DEPTH_STENCIL;
        break;
      case FramebufferTextureFormat::DEPTH:
        textureData->Format = TextureFormat::DEPTH;
        textureData->DataType = TextureDataType::FLOAT;
        break;
      case FramebufferTextureFormat::STENCIL:
        textureData->Format = TextureFormat::STENCIL;
        break;
    }

    return Texture::Create(textureData);
  }
}