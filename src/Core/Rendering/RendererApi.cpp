#include "pch.h"
#include "Core/Rendering/RendererApi.h"
#include "Core/Rendering/Renderer.h"

#if _WIN32
#include "Platform/Direct3D12/D3D12RendererApi.h"
#include "Platform/OpenGL/OpenGLRendererApi.h"
#include "Platform/Vulkan/VulkanRendererApi.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLRendererApi.h"
#include "Platform/Vulkan/VulkanRendererApi.h"
#elif __APPLE__
// #include "Platform/Metal/MetalRendererApi.h"
#endif

namespace Dwarf
{
  std::shared_ptr<RendererApi>
  RendererApi::Create()
  {
    switch (Renderer::GetAPI())
    {
#ifdef _WIN32
      case GraphicsApi::D3D12:
        // return std::make_shared<D3D12RendererApi>();
        break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLRendererApi>();
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanRendererApi>();
        break;
#elif __linux__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLRendererApi>();
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::Vulkan: break;
#elif __APPLE__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::OpenGL: break;
      case GraphicsApi::Metal:
        // return std::make_shared<MetalRendererApi>();
        break;
      case GraphicsApi::Vulkan: break;
#endif
    }
    return nullptr;
  }
}