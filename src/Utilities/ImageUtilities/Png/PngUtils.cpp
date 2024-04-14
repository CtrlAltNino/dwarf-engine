#include "dpch.h"

#include "PngUtils.h"
#include <stb_image.h>

namespace Dwarf {
Ref<TextureContainer>
PngUtils::LoadFromPath(std::filesystem::path const &path) {
  spng_ctx *png = spng_ctx_new(0);
  if (!png) {
    // DWARF_CORE_ERROR("Failed to initialize PNG decoder");
    return nullptr;
  }

  FILE *file = fopen(path.string().c_str(), "rb");
  if (!file) {
    // DWARF_CORE_ERROR("Failed to open file: {0}", path.string());
    spng_ctx_free(png);
    return nullptr;
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  unsigned char *pngData = new unsigned char[fileSize];
  fread(pngData, 1, fileSize, file);
  fclose(file);

  spng_set_png_buffer(png, pngData, fileSize);
  spng_set_chunk_limits(png, 1024 * 1024, 1024 * 1024);

  spng_ihdr ihdr;
  spng_get_ihdr(png, &ihdr);

  unsigned char *imageData = new unsigned char[ihdr.width * ihdr.height * 4];
  spng_decode_image(png, imageData, ihdr.height * 4, SPNG_FMT_RGBA8, 0);

  int width;
  int height;
  int channels;
  stbi_uc *data =
      stbi_load(path.string().c_str(), &width, &height, &channels, 0);
  ImageData<unsigned char> imageData;
  imageData.Data =
      std::vector<unsigned char>(data, data + width * height * channels);

  Ref<TextureContainer> textureData = CreateRef<TextureContainer>();
  textureData->Width = width;
  textureData->Height = height;
  textureData->Format = TextureFormat::RGBA;
  textureData->Type = TextureType::TEXTURE_2D;
  // textureData->DataType = ihdr.bit_depth == 8 ?
  // TextureDataType::UNSIGNED_BYTE : TextureDataType::UNSIGNED_SHORT;
  textureData->DataType = TextureDataType::UNSIGNED_BYTE;

  // copy content from data into textureData->ImageData
  textureData->ImageData = imageData;

  spng_ctx_free(png);

  return textureData;
}
} // namespace Dwarf