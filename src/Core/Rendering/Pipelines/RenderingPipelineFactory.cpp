#include "RenderingPipelineFactory.h"

#include "Core/Rendering/Pipelines/Forward/ForwardRenderer.h"
// #include "Core/Rendering/Pipelines/Deferred/DeferredRenderer.h"

namespace Dwarf
{
  RenderingPipelineFactory::RenderingPipelineFactory(
    std::shared_ptr<IRendererApiFactory> rendererApiFactory,
    std::shared_ptr<IMaterialFactory>    materialFactory,
    std::shared_ptr<IShaderFactory>      shaderFactory,
    std::shared_ptr<IShaderSourceCollectionFactory>
      shaderSourceCollectionFactory)
    : m_RendererApi(rendererApiFactory->Create())
    , m_MaterialFactory(materialFactory)
    , m_ShaderFactory(shaderFactory)
    , m_ShaderSourceCollectionFactory(shaderSourceCollectionFactory)
  {
  }

  std::shared_ptr<IRenderingPipeline>
  RenderingPipelineFactory::Create(PipelineType type)
  {
    switch (type)
    {
      case PipelineType::Forward:
        return std::make_shared<ForwardRenderer>(
          m_RendererApi,
          m_MaterialFactory,
          m_ShaderFactory,
          m_ShaderSourceCollectionFactory);
      // case PipelineType::Deferred: return
      // std::make_shared<DeferredRenderer>();
      default: return nullptr;
    }
  }
} // namespace Dwarf