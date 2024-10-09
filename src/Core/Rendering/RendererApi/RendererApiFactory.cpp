#include "Core/Rendering/RendererApi/RendererApiFactory.h"
#include "Core/Base.h"

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
  RendererApiFactory::RendererApiFactory(
    std::shared_ptr<IDwarfLogger>   logger,
    GraphicsApi                     api,
    std::shared_ptr<IAssetDatabase> assetDatabase,
    std::shared_ptr<IShaderFactory> shaderFactory,
    std::shared_ptr<IEditorStats>   editorStats)
    : m_GraphicsApi(api)
    , m_AssetDatabase(assetDatabase)
    , m_ShaderFactory(shaderFactory)
    , m_Logger(logger)
    , m_EditorStats(editorStats)
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
        return std::make_shared<OpenGLRendererApi>(
          m_AssetDatabase, m_ShaderFactory, m_Logger, m_EditorStats);
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanShader>();
        break;
#elif __linux__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLRendererApi>(
          m_AssetDatabase, m_ShaderFactory, m_Logger, m_EditorStats);
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
      case GraphicsApi::None:
        throw std::runtime_error("Graphics API not set.");
        break;
    }

    return nullptr;
  }
}