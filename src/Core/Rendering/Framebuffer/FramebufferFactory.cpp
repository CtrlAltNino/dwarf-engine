#include "FramebufferFactory.h"

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
  FramebufferFactory::FramebufferFactory(
    GraphicsApi                      api,
    std::shared_ptr<ITextureFactory> textureFactory)
    : m_Api(api)
    , m_TextureFactory(textureFactory)
  {
  }

  std::shared_ptr<IFramebuffer>
  FramebufferFactory::Create(const FramebufferSpecification& spec)
  {
    switch (m_Api)
    {
#ifdef _WIN32
      case GraphicsApi::D3D12:
        // return std::make_shared<D3D12Framebuffer>(spec);
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLFramebuffer>(spec, m_TextureFactory);
        break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanFramebuffer>(spec);
        break;
#elif __linux__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLFramebuffer>(spec, m_TextureFactory);
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
} // namespace Dwarf