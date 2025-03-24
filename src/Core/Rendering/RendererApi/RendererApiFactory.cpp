#include "pch.h"

#include "Core/Base.h"
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
  RendererApiFactory::RendererApiFactory(
    std::shared_ptr<IDwarfLogger>        logger,
    GraphicsApi                          api,
    std::shared_ptr<IAssetDatabase>      assetDatabase,
    std::shared_ptr<IShaderRegistry>     shaderRegistry,
    std::shared_ptr<IEditorStats>        editorStats,
    std::shared_ptr<IOpenGLStateTracker> stateTracker,
    std::shared_ptr<IShaderSourceCollectionFactory>
                                        shaderSourceCollectionFactory,
    std::shared_ptr<IMeshFactory>       meshFactory,
    std::shared_ptr<IMeshBufferFactory> meshBufferFactory)
    : mGraphicsApi(api)
    , mAssetDatabase(std::move(assetDatabase))
    , mShaderRegistry(std::move(shaderRegistry))
    , mLogger(std::move(logger))
    , mEditorStats(std::move(editorStats))
    , mStateTracker(std::move(stateTracker))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
    , mMeshFactory(std::move(meshFactory))
    , mMeshBufferFactory(std::move(meshBufferFactory))
  {
    mLogger->LogDebug(Log("RendererApiFactory created.", "RendererApiFactory"));
  }

  RendererApiFactory::~RendererApiFactory()
  {
    mLogger->LogDebug(Log("RendererApiFactory created.", "RendererApiFactory"));
  }

  auto
  RendererApiFactory::Create() const -> std::unique_ptr<IRendererApi>
  {
    switch (mGraphicsApi)
    {
#if _WIN32
      case GraphicsApi::OpenGL:
        return std::make_unique<OpenGLRendererApi>(
          mAssetDatabase,
          mShaderRegistry,
          mLogger,
          mEditorStats,
          mStateTracker,
          mShaderSourceCollectionFactory,
          mMeshFactory,
          mMeshBufferFactory);
        break;
      case GraphicsApi::D3D12:
      case GraphicsApi::Metal:
      case GraphicsApi::Vulkan: break;
#elif __linux__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::OpenGL:
        return std::make_unique<OpenGLRendererApi>(
          mAssetDatabase,
          mShaderRegistry,
          mLogger,
          mEditorStats,
          mStateTracker,
          mShaderSourceCollectionFactory,
          mMeshFactory,
          mMeshBufferFactory);
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