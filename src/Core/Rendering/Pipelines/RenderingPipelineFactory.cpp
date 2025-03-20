#include "RenderingPipelineFactory.h"

#include "Core/Rendering/Pipelines/Forward/ForwardRenderer.h"
// #include "Core/Rendering/Pipelines/Deferred/DeferredRenderer.h"

namespace Dwarf
{
  RenderingPipelineFactory::RenderingPipelineFactory(
    std::shared_ptr<IDwarfLogger>        logger,
    std::shared_ptr<IRendererApiFactory> rendererApiFactory,
    std::shared_ptr<IMaterialFactory>    materialFactory,
    std::shared_ptr<IShaderRegistry>     shaderRegistry,
    std::shared_ptr<IShaderSourceCollectionFactory>
                                        shaderSourceCollectionFactory,
    std::shared_ptr<IMeshFactory>       meshFactory,
    std::shared_ptr<IMeshBufferFactory> meshBufferFactory,
    std::shared_ptr<IDrawCallList>      drawCallList)
    : mLogger(logger)
    , mRendererApi(rendererApiFactory->Create())
    , mMaterialFactory(materialFactory)
    , mShaderRegistry(shaderRegistry)
    , mShaderSourceCollectionFactory(shaderSourceCollectionFactory)
    , mMeshFactory(meshFactory)
    , mMeshBufferFactory(meshBufferFactory)
    , mDrawCallList(drawCallList)
  {
    mLogger->LogDebug(
      Log("RenderingPipelineFactory created", "RenderingPipelineFactory"));
  }

  RenderingPipelineFactory::~RenderingPipelineFactory()
  {
    mLogger->LogDebug(
      Log("RenderingPipelineFactory destroyed", "RenderingPipelineFactory"));
  }

  std::shared_ptr<IRenderingPipeline>
  RenderingPipelineFactory::Create(PipelineType type)
  {
    switch (type)
    {
      case PipelineType::Forward:
        return std::make_shared<ForwardRenderer>(mRendererApi,
                                                 mMaterialFactory,
                                                 mShaderRegistry,
                                                 mShaderSourceCollectionFactory,
                                                 mMeshFactory,
                                                 mMeshBufferFactory,
                                                 mDrawCallList);
      // case PipelineType::Deferred: return
      // std::make_shared<DeferredRenderer>();
      default: return nullptr;
    }
  }
} // namespace Dwarf