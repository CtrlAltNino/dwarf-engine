#include "pch.h"

#include "Core/Rendering/Pipelines/RenderingPipeline.h"
#include "RenderingPipelineFactory.h"

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
    std::shared_ptr<IDrawCallWorkerFactory> drawCallWorkerFactory)
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
                                               mMaterialFactory,
                                               mShaderRegistry,
                                               mShaderSourceCollectionFactory,
                                               mMeshFactory,
                                               mMeshBufferFactory,
                                               mFramebufferFactory,
                                               mDrawCallListFactory,
                                               mDrawCallWorkerFactory);
  }
} // namespace Dwarf