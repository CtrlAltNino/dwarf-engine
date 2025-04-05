#include "pch.h"

#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "MeshBufferFactory.h"
#include "Platform/OpenGL/OpenGLMesh.h"

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
    mLogger->LogDebug(Log("MeshBufferFactory created", "MeshBufferFactory"));
  }

  MeshBufferFactory::~MeshBufferFactory()
  {
    mLogger->LogDebug(Log("MeshBufferFactory destroyed", "MeshBufferFactory"));
  }

  auto
  MeshBufferFactory::Create(const std::unique_ptr<IMesh>& mesh) const
    -> std::unique_ptr<IMeshBuffer>
  {
    mLogger->LogDebug(Log("Creating MeshBuffer", "MeshBufferFactory"));

    // Creating a shader based on the graphics API.
    switch (mGraphicsApi)
    {
      using enum GraphicsApi;
      case None:
        mLogger->LogError(Log("Graphics API is not set", "MeshBufferFactory"));
        throw std::runtime_error("Graphics API is not set");
      case Vulkan:
        mLogger->LogError(
          Log("Vulkan API has not been implemented yet", "MeshBufferFactory"));
        throw std::runtime_error("Vulkan API has not been implemented yet");
      case OpenGL:
        return std::make_unique<OpenGLMesh>(
          mesh->GetVertices(), mesh->GetIndices(), mLogger, mVramTracker);
      case D3D12:
#ifdef _WIN32
        mLogger->LogError(Log("Direct3D12 API has not been implemented yet",
                              "MeshBufferFactory"));
        throw std::runtime_error("Direct3D12 API has not been implemented yet");
#elif __linux__
        mLogger->LogError(
          Log("Direct3D12 is only supported on Windows", "MeshBufferFactory"));
        throw std::runtime_error("Direct3D12 is only supported on Windows");
#endif
    }

    mLogger->LogError(Log("Failed to create MeshBuffer", "MeshBufferFactory"));

    return nullptr;
  }
}