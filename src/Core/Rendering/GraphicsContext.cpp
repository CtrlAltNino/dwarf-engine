#include "dpch.h"

#include "Core/Rendering/GraphicsContext.h"
#include "Core/Rendering/Renderer.h"

#ifdef _WIN32
// #include "Platform/OpenGL/D3D12Context.h" - NOT SUPPORTED YET
#include "Platform/OpenGL/OpenGLContext.h"
// #include "Platform/OpenGL/VulkanContext.h" - NOT SUPPORTED YET
#elif __linux__
#include "Platform/OpenGL/OpenGLContext.h"
// #include "Platform/Vulkan/VulkanContext.h" - NOT SUPPORTED YET
#elif __APPLE__
// #include "Platform/Metal/MetalContext.h" - NOT SUPPORTED YET
#endif

namespace Dwarf
{
  std::unique_ptr<GraphicsContext>
  GraphicsContext::Create(SDL_Window* window)
  {
    switch (Renderer::GetAPI())
    {
#ifdef _WIN32
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
        // std::make_unique<VulkanContext>(static_cast<SDL_Window*>(window));
        // - NOT SUPPORTED YET
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
}
