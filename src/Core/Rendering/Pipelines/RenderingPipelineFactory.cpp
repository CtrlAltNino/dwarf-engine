#include "pch.hpp"

#include "Core/Rendering/Pipelines/RenderingPipeline.hpp"
#include "RenderingPipelineFactory.hpp"

namespace Dwarf
{
  RenderingPipelineFactory::RenderingPipelineFactory(
    std::shared_ptr<IDwarfLogger>               logger,
    const std::shared_ptr<IRendererApiFactory>& rendererApiFactory,
    std::shared_ptr<IMaterialFactory>           materialFactory,
    std::shared_ptr<IShaderRegistry>            shaderRegistry,
    std::shared_ptr<IShaderSourceCollectionFactory>
                                            shaderSourceCollectionFactory,
    std::shared_ptr<IMeshFactory>           meshFactory,
    std::shared_ptr<IMeshBufferFactory>     meshBufferFactory,
    std::shared_ptr<IFramebufferFactory>    framebufferFactory,
    std::shared_ptr<IDrawCallListFactory>   drawCallListFactory,
    std::shared_ptr<IDrawCallWorkerFactory> drawCallWorkerFactory,
    std::shared_ptr<IPingPongBufferFactory> pingPongBufferFactory,
    std::shared_ptr<ILoadedScene>           loadedScene,
    std::shared_ptr<ISkyboxRenderer>        skyboxRenderer)
    : mLogger(std::move(logger))
    , mRendererApi(rendererApiFactory->Create())
    , mMaterialFactory(std::move(materialFactory))
    , mShaderRegistry(std::move(shaderRegistry))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
    , mMeshFactory(std::move(meshFactory))
    , mMeshBufferFactory(std::move(meshBufferFactory))
    , mFramebufferFactory(std::move(framebufferFactory))
    , mDrawCallListFactory(std::move(drawCallListFactory))
    , mDrawCallWorkerFactory(std::move(drawCallWorkerFactory))
    , mPingPongBufferFactory(std::move(pingPongBufferFactory))
    , mLoadedScene(std::move(loadedScene))
    , mSkyboxRenderer(std::move(skyboxRenderer))
  {
    mLogger->LogDebug(
      Log("RenderingPipelineFactory created", "RenderingPipelineFactory"));
  }

  RenderingPipelineFactory::~RenderingPipelineFactory()
  {
    mLogger->LogDebug(
      Log("RenderingPipelineFactory destroyed", "RenderingPipelineFactory"));
  }

  auto
  RenderingPipelineFactory::Create() const
    -> std::unique_ptr<IRenderingPipeline>
  {
    return std::make_unique<RenderingPipeline>(mRendererApi,
                                               mShaderRegistry,
                                               mShaderSourceCollectionFactory,
                                               mMeshFactory,
                                               mMeshBufferFactory,
                                               mLoadedScene,
                                               mSkyboxRenderer,
                                               mFramebufferFactory,
                                               mMaterialFactory,
                                               mDrawCallListFactory,
                                               mDrawCallWorkerFactory,
                                               mPingPongBufferFactory);
  }
} // namespace Dwarf