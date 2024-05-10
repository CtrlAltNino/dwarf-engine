

#include "Core/Rendering/Framebuffer.h"
#include "Core/Rendering/Renderer.h"

#ifdef _WIN32
// #include "Platform/Direct3D12/D3D12Framebuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
// #include "Platform/Vulkan/VulkanFramebuffer.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLFramebuffer.h"
// #include "Platform/Vulkan/VulkanFramebuffer.h"
#elif __APPLE__
// #include "Platform/Metal/MetalFramebuffer.h"
#endif

namespace Dwarf
{
  std::shared_ptr<Framebuffer>
  Framebuffer::Create(const FramebufferSpecification& spec)
  {
    switch (Renderer::GetAPI())
    {
#ifdef _WIN32
      case GraphicsApi::D3D12:
        // return std::make_shared<D3D12Framebuffer>(spec);
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLFramebuffer>(spec);
        break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanFramebuffer>(spec);
        break;
#elif __linux__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLFramebuffer>(spec);
        break;
      case GraphicsApi::Vulkan: break;
#elif __APPLE__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::Metal:
        // return std::make_shared<MetalFramebuffer>(spec);
        break;
      case GraphicsApi::OpenGL: break;
      case GraphicsApi::Vulkan: break;
#endif
    }
    return nullptr;
  }
}