#include "pch.h"

#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "MeshBufferFactory.h"

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
    , mLogger(std::move(logger))
    , mVramTracker(std::move(vramTracker))
  {
    mLogger->LogDebug(Log("MeshBufferFactory created.", "MeshBufferFactory"));
  }

  MeshBufferFactory::~MeshBufferFactory()
  {
    mLogger->LogDebug(Log("MeshBufferFactory destroyed.", "MeshBufferFactory"));
  }

  auto
  MeshBufferFactory::Create(const std::unique_ptr<IMesh>& mesh) const
    -> std::unique_ptr<IMeshBuffer>
  {
    // Creating a shader based on the graphics API.
    switch (mGraphicsApi)
    {
      using enum GraphicsApi;
      case None:
      case D3D12: break;
      case OpenGL:
        return std::make_unique<OpenGLMesh>(
          mesh->GetVertices(), mesh->GetIndices(), mLogger, mVramTracker);
        break;
      case Metal:
      case Vulkan: break;
    }

    return nullptr;
  }
}