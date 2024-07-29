#include "GraphicsContextFactory.h"

#if _WIN32
#include "Platform/OpenGL/OpenGLContext.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLContext.h"
#elif __APPLE__
// #include "Platform/Metal/MetalContext.h"
#endif

namespace Dwarf
{
  GraphicsContextFactory::GraphicsContextFactory(GraphicsApi api)
    : m_Api(api)
  {
  }
  std::unique_ptr<IGraphicsContext>
  GraphicsContextFactory::Create(SDL_Window* window)
  {
    switch (m_Api)
    {
#if _WIN32
      case GraphicsApi::D3D12:
        // return
        // std::make_unique<D3D12Context>(static_cast<SDL_Window*>(window)); -
        // NOT SUPPORTED YET
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::OpenGL:
        return std::make_unique<OpenGLContext>(window);
        break;
      case GraphicsApi::Vulkan:
        // return
        // std::make_unique<VulkanContext>(static_cast<SDL_Window*>(window)); -
        // NOT SUPPORTED YET
        break;
#elif __linux__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::OpenGL:
        return std::make_unique<OpenGLContext>(window);
        break;
      case GraphicsApi::Vulkan:
        // return std::make_unique<VulkanContext>(static_cast<SDL_Window
        // *>(window)); -NOT SUPPORTED YET
        break;
#elif __APPLE__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::Metal:
        // return
        // std::make_unique<MetalContext>(static_cast<SDL_Window*>(window)); -
        // NOT SUPPORTED YET
        break;
      case GraphicsApi::OpenGL: break;
      case GraphicsApi::Vulkan: break;
#endif
    }

    return nullptr;
  }
} // namespace Dwarf