#include "pch.h"

#include "Core/Base.h"
#include "Core/Rendering/RendererApi/RendererApiFactory.h"
#include "Platform/OpenGL/OpenGLRendererApi.h"

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
    mLogger->LogDebug(Log("RendererApiFactory created", "RendererApiFactory"));
  }

  RendererApiFactory::~RendererApiFactory()
  {
    mLogger->LogDebug(Log("RendererApiFactory created", "RendererApiFactory"));
  }

  auto
  RendererApiFactory::Create() const -> std::unique_ptr<IRendererApi>
  {
    mLogger->LogDebug(Log("Creating RendererApi", "RendererApiFactory"));

    switch (mGraphicsApi)
    {
      using enum GraphicsApi;
      case None:
        mLogger->LogError(Log("Graphics API is not set", "RendererApiFactory"));
        throw std::runtime_error("Graphics API is not set");
      case OpenGL:
        return std::make_unique<OpenGLRendererApi>(
          mAssetDatabase,
          mShaderRegistry,
          mLogger,
          mEditorStats,
          mStateTracker,
          mShaderSourceCollectionFactory,
          mMeshFactory,
          mMeshBufferFactory);
      case Vulkan:
        mLogger->LogError(
          Log("Vulkan API has not been implemented yet", "RendererApiFactory"));
        throw std::runtime_error("Vulkan API has not been implemented yet");
      case D3D12:
#ifdef _WIN32
        mLogger->LogError(Log("Direct3D12 API has not been implemented yet",
                              "RendererApiFactory"));
        throw std::runtime_error("Direct3D12 API has not been implemented yet");
#elif __linux__
        mLogger->LogError(
          Log("Direct3D12 is only supported on Windows", "RendererApiFactory"));
        throw std::runtime_error("Direct3D12 is only supported on Windows");
#endif
    }

    mLogger->LogError(
      Log("Failed to create RendererApi", "RendererApiFactory"));

    return nullptr;
  }
}