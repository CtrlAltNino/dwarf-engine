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
  Ref<Texture>
  TextureCreator::FromPath(std::filesystem::path const& path)
  {
    std::string            ext = path.extension().string();
    Ref<TextureParameters> parameters = GetParameters(path);

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

  Ref<TextureParameters>
  TextureCreator::GetParameters(std::filesystem::path const& path)
  {
    Ref<TextureParameters> parameters = CreateRef<TextureParameters>();
    parameters->WrapR = TextureWrap::REPEAT;
    parameters->WrapS = TextureWrap::REPEAT;
    parameters->WrapT = TextureWrap::REPEAT;
    parameters->MinFilter = TextureMinFilter::LINEAR;
    parameters->MagFilter = TextureMagFilter::LINEAR;
    return parameters;
  }

  Ref<Texture>
  TextureCreator::FromPng(std::filesystem::path const& path,
                          Ref<TextureParameters>       parameters)
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

    Ref<TextureContainer> textureData = CreateRef<TextureContainer>();
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

    Ref<Texture> texture = Texture::Create(textureData, parameters);

    spng_ctx_free(png);

    return texture;
  }

  Ref<Texture>
  TextureCreator::FromJpeg(std::filesystem::path const& path,
                           Ref<TextureParameters>       parameters)
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

    Ref<TextureContainer> data = CreateRef<TextureContainer>();
    data->Width = width;
    data->Height = height;
    data->Format = TextureFormat::RGB;
    data->Type = TextureType::TEXTURE_2D;
    data->DataType = TextureDataType::UNSIGNED_BYTE;
    data->ImageData = imageData;

    return Texture::Create(data, parameters);
  }

  Ref<Texture>
  TextureCreator::FromTga(std::filesystem::path const& path,
                          Ref<TextureParameters>       parameters)
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

    // Ref<TextureContainer> textureData = CreateRef<TextureContainer>();
    // textureData->Width = width;
    // textureData->Height = height;
    // textureData->Format = TextureFormat::RGBA;
    // textureData->Type = TextureType::TEXTURE_2D;
    // textureData->DataType = TextureDataType::UNSIGNED_BYTE;

    // // copy content from data into textureData->ImageData
    // textureData->ImageData = data;

    // return Texture::Create(parameters, textureData);
    return nullptr;
  }

  Ref<Texture>
  TextureCreator::FromHdr(std::filesystem::path const& path,
                          Ref<TextureParameters>       parameters)
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

    // Ref<TextureContainer> textureData = CreateRef<TextureContainer>();
    // textureData->Width = width;
    // textureData->Height = height;
    // textureData->Format = TextureFormat::RGBA;
    // textureData->Type = TextureType::TEXTURE_2D;
    // textureData->DataType = TextureDataType::UNSIGNED_BYTE;

    // // copy content from data into textureData->ImageData
    // textureData->ImageData = data;

    // return Texture::Create(parameters, textureData);
    return nullptr;
  }

  Ref<Texture>
  TextureCreator::FromTiff(std::filesystem::path const& path,
                           Ref<TextureParameters>       parameters)
  {
    // TIFF* tiff = TIFFOpen(path.string().c_str(), "r");
    // if (!tiff)
    // {
    //   // DWARF_CORE_ERROR("Failed to open TIFF file: {0}", path.string());
    //   return nullptr;
    // }

    // uint32_t  width;
    // uint32_t  height;
    // uint16_t  bitsPerSample;
    // uint16_t  samplesPerPixel;
    // uint16_t  sampleFormat;
    // uint16_t  compression;
    // uint16_t  photometric;
    // uint16_t  planarConfig;
    // uint16_t  orientation;
    // uint16_t  rowsPerStrip;
    // uint16_t* red;
    // uint16_t* green;
    // uint16_t* blue;
    // uint16_t* alpha;

    // TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width);
    // TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height);
    // TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);
    // TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel);
    // TIFFGetField(tiff, TIFFTAG_SAMPLEFORMAT, &sampleFormat);
    // TIFFGetField(tiff, TIFFTAG_COMPRESSION, &compression);
    // TIFFGetField(tiff, TIFFTAG_PHOTOMETRIC, &photometric);
    // TIFFGetField(tiff, TIFFTAG_PLANARCONFIG, &planarConfig);
    // TIFFGetField(tiff, TIFFTAG_ORIENTATION, &orientation);
    // TIFFGetField(tiff, TIFFTAG_ROWSPERSTRIP, &rowsPerStrip);

    // uint32_t* raster =
    //   (uint32_t*)_TIFFmalloc(width * height * sizeof(uint32_t));
    // if (!raster)
    // {
    //   // DWARF_CORE_ERROR("Failed to allocate memory for TIFF image");
    //   TIFFClose(tiff);
    //   return nullptr;
    // }

    // if (!TIFFReadRGBAImage(tiff, width, height, raster, 0))
    // {
    //   // DWARF_CORE_ERROR("Failed to read TIFF image");
    //   _TIFFfree(raster);
    //   TIFFClose(tiff);
    //   return nullptr;
    // }

    // _TIFFfree(raster);
    // TIFFClose(tiff);

    return nullptr;
  }

  Ref<Texture>
  TextureCreator::FromBmp(std::filesystem::path const& path,
                          Ref<TextureParameters>       parameters)
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

    // Ref<TextureContainer> textureData = CreateRef<TextureContainer>();
    // textureData->Width = width;
    // textureData->Height = height;
    // textureData->Format = TextureFormat::RGBA;
    // textureData->Type = TextureType::TEXTURE_2D;
    // textureData->DataType = TextureDataType::UNSIGNED_BYTE;

    // // copy content from data into textureData->ImageData
    // textureData->ImageData = data;

    // return Texture::Create(parameters, textureData);
    return nullptr;
  }

  Ref<Texture>
  TextureCreator::FromData(TextureParameters const& parameters,
                           glm::ivec2               size,
                           void*                    data)
  {
    Ref<TextureContainer> textureData = CreateRef<TextureContainer>();
    textureData->Width = size.x;
    textureData->Height = size.y;
    textureData->Format = TextureFormat::RGBA;
    textureData->Type = TextureType::TEXTURE_2D;
    textureData->DataType = TextureDataType::UNSIGNED_BYTE;
    textureData->ImageData = (unsigned char*)data;

    return Texture::Create(textureData,
                           CreateRef<TextureParameters>(parameters));
  }

  Ref<Texture>
  TextureCreator::Empty(glm::ivec2 size)
  {
    Ref<TextureContainer> textureData = CreateRef<TextureContainer>();
    textureData->Width = size.x;
    textureData->Height = size.y;
    textureData->Format = TextureFormat::RGBA;
    textureData->Type = TextureType::TEXTURE_2D;
    textureData->DataType = TextureDataType::UNSIGNED_BYTE;
    textureData->ImageData = nullptr;

    return Texture::Create(textureData);
  }

  Ref<Texture>
  TextureCreator::Empty(FramebufferTextureSpecification const& parameters,
                        glm::ivec2                             size)
  {
    Ref<TextureContainer> textureData = CreateRef<TextureContainer>();
    textureData->Width = size.x;
    textureData->Height = size.y;
    textureData->Format = TextureFormat::RGBA;
    textureData->Type = TextureType::TEXTURE_2D;
    textureData->DataType = TextureDataType::UNSIGNED_BYTE;
    textureData->ImageData = nullptr;

    switch (parameters.TextureFormat)
    {
      case FramebufferTextureFormat::RGBA8:
        textureData->Format = TextureFormat::RGBA;
        break;
      case FramebufferTextureFormat::RED_INTEGER:
        textureData->Format = TextureFormat::RED;
        break;
      case FramebufferTextureFormat::DEPTH24STENCIL8:
        textureData->Format = TextureFormat::DEPTH_STENCIL;
        break;
    }

    return Texture::Create(textureData);
  }
}