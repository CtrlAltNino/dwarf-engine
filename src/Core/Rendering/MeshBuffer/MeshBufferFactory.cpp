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
    : mGraphicsApi(graphicsApi)
    , mLogger(logger)
    , mVramTracker(vramTracker)
  {
    mLogger->LogDebug(Log("MeshBufferFactory created.", "MeshBufferFactory"));
  }

  MeshBufferFactory::~MeshBufferFactory()
  {
    mLogger->LogDebug(Log("MeshBufferFactory destroyed.", "MeshBufferFactory"));
  }

  std::unique_ptr<IMeshBuffer>
  MeshBufferFactory::Create(std::unique_ptr<IMesh>& mesh)
  {
    mLogger->LogDebug(Log("Creating mesh.", "MeshBufferFactory"));
    // Creating a shader based on the graphics API.
    switch (mGraphicsApi)
    {
      using enum GraphicsApi;
      case None: break;
#if _WIN32
      case D3D12:
        // return std::make_shared<D3D12Shader>();
        break;
      case OpenGL:
        return std::make_unique<OpenGLMesh>(
          mesh->GetVertices(), mesh->GetIndices(), mLogger, mVramTracker);
        break;
      case Metal: break;
      case Vulkan:
        // return std::make_shared<VulkanShader>();
        break;
#elif __linux__
      case D3D12: break;
      case OpenGL:
        return std::make_unique<OpenGLMesh>(
          mesh->GetVertices(), mesh->GetIndices(), mLogger, mVramTracker);
        break;
      case Metal: break;
      case Vulkan:
        // return std::make_shared<VulkanShader>();
        break;
#elif __APPLE__
      case D3D12: break;
      case OpenGL: break;
      case Metal:
        // return std::make_shared<MetalShader>();
        break;
      case Vulkan: break;
#endif
    }

    return nullptr;
  }
}