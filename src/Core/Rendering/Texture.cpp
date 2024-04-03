#include "dpch.h"
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <tiffio.h>
#include <spng.h>

#include "Core/Rendering/Renderer.h"

#ifdef WIN32
// #include "Platform/Direct3D12/D3D12Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"
// #include "Platform/Vulkan/VulkanTexture.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLTexture.h"
// #include "Platform/Vulkan/VulkanTexture.h"
#elif __APPLE__
// #include "Platform/Metal/MetalTexture.h"
#endif

#include <nlohmann/json.hpp>

namespace Dwarf
{
    Ref<Texture> Texture::LoadFromJpg(const std::filesystem::path &path)
    {
        int width;
        int height;
        int channels;
        stbi_uc *data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            // DWARF_CORE_ERROR("Failed to load texture from file: {0}", path.string());
            return nullptr;
        }

        // TODO: Handle metadata

        TextureParameters parameters{
            TextureType::TEXTURE_2D,
            TextureFormat::RGB,
            TextureDataType::UNSIGNED_BYTE,
            TextureWrap::REPEAT,
            TextureWrap::REPEAT,
            TextureWrap::REPEAT,
            TextureMinFilter::LINEAR,
            TextureMagFilter::LINEAR,
        };

        Ref<Texture> texture = CreateRef<OpenGLTexture>(parameters, glm::ivec3(width, height, 0), (void *)data);
        stbi_image_free(data);

        return texture;
    }

    Ref<Texture> Texture::LoadFromPng(std::filesystem::path const &path)
    {
        int width;
        int height;
        int channels;
        TextureFormat format;

        // Open the file for reading
        std::ifstream inputFile("example.png", std::ios::binary);

        if (inputFile)
        {
            // Determine the file size
            inputFile.seekg(0, std::ios::end);
            std::streampos fileSize = inputFile.tellg();
            inputFile.seekg(0, std::ios::beg);

            // Allocate a buffer to hold the file contents
            std::vector<unsigned char> buffer(fileSize);

            // Read the file into the buffer
            inputFile.read(reinterpret_cast<char *>(buffer.data()), fileSize);

            // Close the file
            inputFile.close();

            // Decode the PNG image using libspng
            spng_ctx *ctx = spng_ctx_new(0);
            spng_set_png_buffer(ctx, buffer.data(), buffer.size());
            // spng_decode_image(ctx, SPNG_FMT_RGBA8, NULL, 0);

            // Get information about the image
            struct spng_ihdr ihdr;
            spng_get_ihdr(ctx, &ihdr);

            std::cout << "Width: " << ihdr.width << std::endl;
            std::cout << "Height: " << ihdr.height << std::endl;

            // Clean up
            spng_ctx_free(ctx);
        }
        else
        {
            std::cerr << "Unable to open file" << std::endl;
        }

        // stbi_uc *data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
        /*if (!data)
        {
            // DWARF_CORE_ERROR("Failed to load texture from file: {0}", path.string());
            return nullptr;
        }*/

        /*if (channels == 3)
            format = TextureFormat::RGB;
        else if (channels == 4)
            format = TextureFormat::RGBA;
        else
            format = TextureFormat::RGB;*/

        // TODO: Handle metadata

        TextureParameters parameters{
            TextureType::TEXTURE_2D,
            format,
            TextureDataType::UNSIGNED_BYTE,
            TextureWrap::REPEAT,
            TextureWrap::REPEAT,
            TextureWrap::REPEAT,
            TextureMinFilter::LINEAR,
            TextureMagFilter::LINEAR,
        };

        // Ref<Texture> texture = CreateRef<OpenGLTexture>(parameters, glm::ivec2(width, height), data);
        // stbi_image_free(data);

        return nullptr;
    }

    Ref<Texture> Texture::LoadFromBmp(std::filesystem::path const &path)
    {
        int width;
        int height;
        int channels;
        stbi_uc *data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            // DWARF_CORE_ERROR("Failed to load texture from file: {0}", path.string());
            return nullptr;
        }

        return nullptr;
    }

    Ref<Texture> Texture::LoadFromTga(std::filesystem::path const &path)
    {
        int width;
        int height;
        int channels;
        stbi_uc *data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            // DWARF_CORE_ERROR("Failed to load texture from file: {0}", path.string());
            return nullptr;
        }

        return nullptr;
    }

    Ref<Texture> Texture::LoadFromHdr(std::filesystem::path const &path)
    {
        int width;
        int height;
        int channels;
        stbi_uc *data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            // DWARF_CORE_ERROR("Failed to load texture from file: {0}", path.string());
            return nullptr;
        }

        return nullptr;
    }

    Ref<Texture> Texture::LoadFromTiff(std::filesystem::path const &path)
    {
        int width;
        int height;
        int channels;
        stbi_uc *data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            // DWARF_CORE_ERROR("Failed to load texture from file: {0}", path.string());
            return nullptr;
        }

        return nullptr;
    }

    // @brief Load a texture data from a file.
    // @param path The path to the texture file.

    Ref<Texture> Texture::Create(std::filesystem::path const &path)
    {
        std::string ext = path.extension().string();

        if (ext == ".jpg" || ext == ".jpeg")
            return Texture::LoadFromJpg(path);
        else if (ext == ".png")
            return Texture::LoadFromPng(path);
        else if (ext == ".bmp")
            return Texture::LoadFromBmp(path);
        else if (ext == ".tga")
            return Texture::LoadFromTga(path);
        else if (ext == ".hdr" || ext == ".exr")
            return Texture::LoadFromHdr(path);
        else if (ext == ".tiff" || ext == ".tif")
            return Texture::LoadFromTiff(path);
        else
        {
            // DWARF_CORE_ERROR("Failed to load texture from file: {0}", path.string());
            return nullptr;
        }
    }

    Ref<Texture> Texture::CreateGpuTexture(TextureParameters const &parameters, glm::ivec3 size, void *data)
    {
        switch (Renderer::GetAPI())
        {
            using enum GraphicsApi;
        case D3D12:
#ifdef WIN32
            // Return D3D12 Texture
#endif
            break;
        case Metal:
#ifdef __APPLE__
            // Return Metal Texture
#endif
            break;
        case OpenGL:
#if defined(__linux__) || defined(WIN32)
        {
            Ref<Texture> texture = CreateRef<OpenGLTexture>(OpenGLTexture(parameters, size, data));
            return texture;
        }
#endif
        break;
        case Vulkan:
#if defined(__linux__) || defined(WIN32)
            // Return Vulkan Texture
#endif
            break;
        }

        return nullptr;
    }
}