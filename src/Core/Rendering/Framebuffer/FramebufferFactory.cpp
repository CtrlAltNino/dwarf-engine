#include "FramebufferFactory.h"
#include "Core/Base.h"
#include <memory>

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
    std::shared_ptr<IDwarfLogger>    logger,
    GraphicsApi                      api,
    std::shared_ptr<ITextureFactory> textureFactory,
    std::shared_ptr<IVramTracker>    vramTracker)
    : m_Logger(logger)
    , m_Api(api)
    , m_TextureFactory(textureFactory)
    , m_VramTracker(vramTracker)
  {
    m_Logger->LogDebug(Log("FramebufferFactory created", "FramebufferFactory"));
  }

  std::shared_ptr<IFramebuffer>
  FramebufferFactory::Create(const FramebufferSpecification& spec)
  {
    m_Logger->LogDebug(Log("Creating framebuffer", "FramebufferFactory"));

    switch (m_Api)
    {
      using enum GraphicsApi;
      case None: break;
#ifdef _WIN32
      case D3D12:
        // return std::make_shared<D3D12Framebuffer>(spec);
        break;
      case Metal: break;
      case OpenGL:
        return std::make_shared<OpenGLFramebuffer>(
          m_Logger, spec, m_TextureFactory, m_VramTracker);
        break;
      case Vulkan:
        // return std::make_shared<VulkanFramebuffer>(spec);
        break;
#elif __linux__
      case D3D12: break;
      case Metal: break;
      case OpenGL:
        return std::make_unique<OpenGLFramebuffer>(
          m_Logger, spec, m_TextureFactory, m_VramTracker);
        break;
      case Vulkan: break;
#elif __APPLE__
      case D3D12: break;
      case Metal:
        // return std::make_shared<MetalFramebuffer>(spec);
        break;
      case OpenGL: break;
      case Vulkan: break;
#endif
    }
    return nullptr;
  }
} // namespace Dwarf