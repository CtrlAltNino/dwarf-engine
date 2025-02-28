#include "MeshBufferFactory.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"

#if _WIN32
#include "Platform/OpenGL/OpenGLMesh.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLMesh.h"
#elif __APPLE__
// #include "Platform/Metal/MetalShader.h"
#endif

namespace Dwarf
{
  MeshBufferFactory::MeshBufferFactory(
    GraphicsApi                   graphicsApi,
    std::shared_ptr<IDwarfLogger> logger,
    std::shared_ptr<IVramTracker> vramTracker)
    : m_GraphicsApi(graphicsApi)
    , m_Logger(logger)
    , m_VramTracker(vramTracker)
  {
    m_Logger->LogDebug(Log("MeshBufferFactory created.", "MeshBufferFactory"));
  }

  MeshBufferFactory::~MeshBufferFactory()
  {
    m_Logger->LogDebug(
      Log("MeshBufferFactory destroyed.", "MeshBufferFactory"));
  }

  std::unique_ptr<IMeshBuffer>
  MeshBufferFactory::Create(IMesh& mesh)
  {
    m_Logger->LogDebug(Log("Creating mesh.", "MeshBufferFactory"));
    // Creating a shader based on the graphics API.
    switch (m_GraphicsApi)
    {
#if _WIN32
      case GraphicsApi::D3D12:
        // return std::make_shared<D3D12Shader>();
        break;
      case GraphicsApi::OpenGL:
        return std::make_unique<OpenGLMesh>(
          mesh.GetVertices(), mesh.GetIndices(), m_Logger, m_VramTracker);
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanShader>();
        break;
#elif __linux__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::OpenGL:
        return std::make_unique<OpenGLMesh>(
          mesh.GetVertices(), mesh.GetIndices(), m_Logger, m_VramTracker);
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanShader>();
        break;
#elif __APPLE__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::OpenGL: break;
      case GraphicsApi::Metal:
        // return std::make_shared<MetalShader>();
        break;
      case GraphicsApi::Vulkan: break;
#endif
    }

    return nullptr;
  }
}