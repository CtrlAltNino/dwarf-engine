#include "Core/Rendering/RendererApi/RendererApiFactory.h"

// Including the shader header files of the graphics API.
#if _WIN32
#include "Platform/OpenGL/OpenGLRendererApi.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLRendererApi.h"
#elif __APPLE__
// #include "Platform/Metal/MetalRendererApi.h"
#endif

namespace Dwarf
{
  RendererApiFactory::RendererApiFactory(GraphicsApi api)
    : m_GraphicsApi(api)
  {
  }

  std::shared_ptr<IRendererApi>
  RendererApiFactory::Create()
  {
    switch (m_GraphicsApi)
    {
#if _WIN32
      case GraphicsApi::D3D12:
        // return std::make_shared<D3D12Shader>();
        break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLRendererApi>();
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanShader>();
        break;
#elif __linux__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLRendererApi>();
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanShader>();
        break;
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