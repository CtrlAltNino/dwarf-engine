#include "dpch.h"

#include "JpegUtils.h"

namespace Dwarf {
  Ref<TextureContainer> JpegUtils::LoadFromPath(
    std::filesystem::path const& path)
  {
    tjhandle jpegDecompressor = tjInitDecompress();
    if (!jpegDecompressor) {
      // DWARF_CORE_ERROR("Failed to initialize JPEG decompressor");
      return nullptr;
    }

    FILE* file = fopen(path.string().c_str(), "rb");
    if (!file) {
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

    Ref<TextureContainer> textureData = CreateRef<TextureContainer>();
    textureData->Width = width;
    textureData->Height = height;
    textureData->Format = TextureFormat::RGB;
    textureData->Type = TextureType::TEXTURE_2D;
    textureData->DataType = TextureDataType::UNSIGNED_BYTE;
    textureData->ImageData = imageData;

    return textureData;
  }
} // namespace Dwarf