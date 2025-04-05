#include "pch.h"

#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "ShaderFactory.h"

namespace Dwarf
{
  ShaderFactory::ShaderFactory(
    GraphicsApi                   graphicsApi,
    std::shared_ptr<IDwarfLogger> logger,
    std::shared_ptr<IShaderSourceCollectionFactory>
      shaderSourceCollectionFactory,
    std::shared_ptr<IShaderParameterCollectionFactory>
                                  shaderParameterCollectionFactory,
    std::shared_ptr<IVramTracker> vramTracker)
    : mGraphicsApi(graphicsApi)
    , mLogger(std::move(logger))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
    , mShaderParameterCollectionFactory(
        std::move(shaderParameterCollectionFactory))
    , mVramTracker(std::move(vramTracker))
  {
    mLogger->LogDebug(Log("ShaderFactory created", "ShaderFactory"));
  }

  ShaderFactory::~ShaderFactory()
  {
    mLogger->LogDebug(Log("ShaderFactory destroyed", "ShaderFactory"));
  }

  auto
  ShaderFactory::Create(std::unique_ptr<IShaderSourceCollection> shaderSources)
    const -> std::shared_ptr<IShader>
  {
    mLogger->LogDebug(Log("Creating shader from sources", "ShaderFactory"));

    // Creating a shader based on the graphics API.
    switch (mGraphicsApi)
    {
      using enum GraphicsApi;
      case None:
        mLogger->LogError(Log("Graphics API is not set", "ShaderFactory"));
        throw std::runtime_error("Graphics API is not set");
      case OpenGL:
        return std::make_shared<OpenGLShader>(std::move(shaderSources),
                                              mShaderParameterCollectionFactory,
                                              mLogger,
                                              mVramTracker);
      case Vulkan:
        mLogger->LogError(
          Log("Vulkan API has not been implemented yet", "ShaderFactory"));
        throw std::runtime_error("Vulkan API has not been implemented yet");
      case D3D12:
#ifdef _WIN32
        mLogger->LogError(
          Log("Direct3D12 API has not been implemented yet", "ShaderFactory"));
        throw std::runtime_error("Direct3D12 API has not been implemented yet");
#elif __linux__
        mLogger->LogError(
          Log("Direct3D12 is only supported on Windows", "ShaderFactory"));
        throw std::runtime_error("Direct3D12 is only supported on Windows");
#endif
    }

    mLogger->LogError(Log("Failed to create Shader", "ShaderFactory"));

    return nullptr;
  }
} // namespace Dwarf