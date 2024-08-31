#include "RenderingPipelineFactory.h"

#include "Core/Rendering/Pipelines/Forward/ForwardRenderer.h"
// #include "Core/Rendering/Pipelines/Deferred/DeferredRenderer.h"

namespace Dwarf
{
  RenderingPipelineFactory::RenderingPipelineFactory(
    std::shared_ptr<IRendererApi> rendererApi)
    : m_RendererApi(rendererApi)
  {
  }

  std::shared_ptr<IRenderingPipeline>
  RenderingPipelineFactory::Create(PipelineType type)
  {
    switch (type)
    {
      case PipelineType::Forward:
        return std::make_shared<ForwardRenderer>(m_RendererApi);
      // case PipelineType::Deferred: return
      // std::make_shared<DeferredRenderer>();
      default: return nullptr;
    }
  }
} // namespace Dwarf