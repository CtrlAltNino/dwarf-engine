#pragma once

#include "Core/Base.h"
#include "TextureCommon.h"
#include <spng.h>
#include <filesystem>

namespace Dwarf
{

    class PngUtils
    {
    public:
        static Ref<TextureData> LoadFromPath(std::filesystem::path const &path)
        {
            spng_ctx *png = spng_ctx_new(0);
            if (!png)
            {
                // DWARF_CORE_ERROR("Failed to initialize PNG decoder");
                return nullptr;
            }

            FILE *file = fopen(path.string().c_str(), "rb");
            if (!file)
            {
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

            spng_ctx_free(png);

            Ref<TextureData> textureData = CreateRef<TextureData>();
            textureData->Width = ihdr.width;
            textureData->Height = ihdr.height;
            textureData->Channels = 4;
            textureData->ImageData = imageData;

            return textureData;
        }
    };
}