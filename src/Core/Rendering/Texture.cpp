#include "dpch.h"

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
    Ref<Texture> Texture::Create(std::filesystem::path path)
    {
        switch (Renderer::GetAPI())
        {
#ifdef WIN32
        case GraphicsApi::D3D12:
            // return CreateRef<D3D12Texture>(D3D12Texture(path));
            break;
        case GraphicsApi::Metal:
            break;
        case GraphicsApi::OpenGL:
            return CreateRef<OpenGLTexture>(OpenGLTexture(path));
            break;
        case GraphicsApi::Vulkan:
            // return CreateRef<VulkanTexture>(VulkanTexture(path));
            break;
#elif __linux__
        case GraphicsApi::D3D12:
            break;
        case GraphicsApi::Metal:
            break;
        case GraphicsApi::OpenGL:
            return CreateRef<OpenGLTexture>(OpenGLTexture(path));
            break;
        case GraphicsApi::Vulkan:
            // return CreateRef<VulkanTexture>(VulkanTexture(path));
            break;
#elif __APPLE__
        case GraphicsApi::D3D12:
            break;
        case GraphicsApi::Metal:
            // return CreateRef<MetalTexture>(MetalTexture(path));
            break;
        case GraphicsApi::OpenGL:
            break;
        case GraphicsApi::Vulkan:
            break;
#endif
        }
        return nullptr;
    }
}