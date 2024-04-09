#include "dpch.h"

// Image loading libraries
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <tiffio.h>
#include <spng.h>
#include <turbojpeg.h>

#include "Core/Rendering/Renderer.h"
#include "Utilities/ImageUtilities/Jpeg/JpegUtils.h"
#include "Utilities/ImageUtilities/Png/PngUtils.h"
#include "Utilities/ImageUtilities/Bmp/BmpUtils.h"
#include "Utilities/ImageUtilities/Tiff/TiffUtils.h"
#include "Utilities/ImageUtilities/Hdr/HdrUtils.h"
#include "Utilities/ImageUtilities/Tga/TgaUtils.h"

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
    Ref<Texture> Texture::Create(std::filesystem::path const &path)
    {
        std::string ext = path.extension().string();
        Ref<TextureData> data = nullptr;
        Ref<TextureParameters> parameters = GetParameters(path);

        if (ext == ".jpg" || ext == ".jpeg")
            data = JpegUtils::LoadFromPath(path);
        else if (ext == ".png")
            data = PngUtils::LoadFromPath(path);
        else if (ext == ".bmp")
            data = BmpUtils::LoadFromPath(path);
        else if (ext == ".tga")
            data = TgaUtils::LoadFromPath(path);
        else if (ext == ".hdr" || ext == ".exr")
            data = HdrUtils::LoadFromPath(path);
        else if (ext == ".tiff" || ext == ".tif")
            data = TiffUtils::LoadFromPath(path);

        return CreateGpuTexture(parameters, data);
    }

    Ref<Texture> Texture::CreateGpuTexture(Ref<TextureParameters> parameters, Ref<TextureData> data)
    {
        switch (Renderer::GetAPI())
        {
            using enum GraphicsApi;
        case D3D12:
#ifdef WIN32
            // return CreateRef<D3D12Texture>(D3D12Texture(parameters, data));
#endif
            break;
        case Metal:
#ifdef __APPLE__
            // return CreateRef<MetalTexture>(MetalTexture(parameters, data));
#endif
            break;
        case OpenGL:
#if defined(__linux__) || defined(WIN32)
            return CreateRef<OpenGLTexture>(OpenGLTexture(parameters, data));
#endif
            break;
        case Vulkan:
#if defined(__linux__) || defined(WIN32)
            // return CreateRef<VulkanTexture>(VulkanTexture(parameters, data));
#endif
            break;
        }

        return nullptr;
    }

    Ref<TextureParameters> Texture::GetParameters(std::filesystem::path const &path)
    {
        Ref<TextureParameters> parameters = CreateRef<TextureParameters>();
        parameters->WrapR = TextureWrap::REPEAT;
        parameters->WrapS = TextureWrap::REPEAT;
        parameters->WrapT = TextureWrap::REPEAT;
        parameters->MinFilter = TextureMinFilter::LINEAR;
        parameters->MagFilter = TextureMagFilter::LINEAR;
        return parameters;
    }
}