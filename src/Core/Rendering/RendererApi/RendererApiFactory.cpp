#include "Core/Rendering/RendererApi/RendererApiFactory.h"
#include "Core/Base.h"

// Including the shader header files of the graphics API.
#if _WIN32
#include "Platform/OpenGL/OpenGLRendererApi.h"
#include "Platform/OpenGL/OpenGLStateTracker.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLRendererApi.h"
#include "Platform/OpenGL/OpenGLStateTracker.h"
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
    , mAssetDatabase(assetDatabase)
    , mShaderRegistry(shaderRegistry)
    , mLogger(logger)
    , mEditorStats(editorStats)
    , mStateTracker(stateTracker)
    , mShaderSourceCollectionFactory(shaderSourceCollectionFactory)
    , mMeshFactory(meshFactory)
    , mMeshBufferFactory(meshBufferFactory)
  {
  }

  RendererApiFactory::~RendererApiFactory() {}

  std::shared_ptr<IRendererApi>
  RendererApiFactory::Create()
  {
    switch (mGraphicsApi)
    {
#if _WIN32
      case GraphicsApi::D3D12:
        // return std::make_shared<D3D12Shader>();
        break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLRendererApi>(
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
#elif __linux__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLRendererApi>(
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