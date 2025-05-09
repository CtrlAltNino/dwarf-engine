#include "pch.hpp"

#include "CubemapGeneratorFactory.hpp"
#include "Platform/OpenGL/OpenGLCubemapGenerator.hpp"

namespace Dwarf
{

  CubemapGeneratorFactory::CubemapGeneratorFactory(
    std::shared_ptr<IDwarfLogger>    logger,
    GraphicsApi                      graphicsApi,
    std::shared_ptr<ITextureFactory> textureFactory,
    std::shared_ptr<IShaderRegistry> shaderRegistry,
    std::shared_ptr<IShaderSourceCollectionFactory>
                                         shaderSourceCollectionFactory,
    std::shared_ptr<IMeshFactory>        meshFactory,
    std::shared_ptr<IMeshBufferFactory>  meshBufferFactory,
    std::shared_ptr<IFramebufferFactory> framebufferFactory,
    std::shared_ptr<IRendererApiFactory> rendererApiFactory)
    : mLogger(std::move(logger))
    , mApi(graphicsApi)
    , mTextureFactory(std::move(textureFactory))
    , mShaderRegistry(std::move(shaderRegistry))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
    , mMeshFactory(std::move(meshFactory))
    , mMeshBufferFactory(std::move(meshBufferFactory))
    , mFramebufferFactory(std::move(framebufferFactory))
    , mRendererApiFactory(std::move(rendererApiFactory))
  {
  }

  auto
  CubemapGeneratorFactory::Create() -> std::shared_ptr<ICubemapGenerator>
  {
    mLogger->LogDebug(
      Log("Creating cubemap generator", "CubemapGeneratorFactory"));

    switch (mApi)
    {
      using enum GraphicsApi;
      case None:
        mLogger->LogError(
          Log("Graphics API is not set", "CubemapGeneratorFactory"));
        throw std::runtime_error("Graphics API is not set");
      case OpenGL:
        return std::make_unique<OpenGLCubemapGenerator>(
          mLogger,
          mTextureFactory,
          mShaderRegistry,
          mShaderSourceCollectionFactory,
          mMeshFactory,
          mMeshBufferFactory,
          mFramebufferFactory,
          mRendererApiFactory);
      case Vulkan:
        mLogger->LogError(Log("Vulkan API has not been implemented yet",
                              "CubemapGeneratorFactory"));
        throw std::runtime_error("Vulkan API has not been implemented yet");
      case D3D12:
#ifdef _WIN32
        mLogger->LogError(Log("Direct3D12 API has not been implemented yet",
                              "CubemapGeneratorFactory"));
        throw std::runtime_error("Direct3D12 API has not been implemented yet");
#elif __linux__
        mLogger->LogError(Log("Direct3D12 is only supported on Windows",
                              "CubemapGeneratorFactory"));
        throw std::runtime_error("Direct3D12 is only supported on Windows");
#endif
    }

    mLogger->LogError(
      Log("Failed to create cubemap generator", "CubemapGeneratorFactory"));

    return nullptr;
  }
}