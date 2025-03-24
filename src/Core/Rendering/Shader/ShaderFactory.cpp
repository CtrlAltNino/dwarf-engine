#include "pch.h"

#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "ShaderFactory.h"

// Including the shader header files of the graphics API.
#if _WIN32
#include "Platform/OpenGL/OpenGLShader.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLShader.h"
#elif __APPLE__
// #include "Platform/Metal/MetalShader.h"
#endif

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
#if _WIN32
      case GraphicsApi::D3D12:
        // return std::make_shared<D3D12Shader>();
        break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLShader>(std::move(shaderSources),
                                              mShaderParameterCollectionFactory,
                                              mLogger,
                                              mVramTracker);
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanShader>();
        break;
#elif __linux__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLShader>(std::move(shaderSources),
                                              mShaderParameterCollectionFactory,
                                              mLogger,
                                              mVramTracker);
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
      case GraphicsApi::None:
        mLogger->LogError(Log("Graphics API is not set", "ShaderFactory"));
        throw std::runtime_error("Graphics API is not set");
        break;
    }

    return nullptr;
  }
} // namespace Dwarf