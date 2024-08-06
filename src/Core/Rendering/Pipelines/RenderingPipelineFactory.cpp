#include "RenderingPipelineFactory.h"

#include "Core/Rendering/Pipelines/Forward/ForwardRenderer.h"
// #include "Core/Rendering/Pipelines/Deferred/DeferredRenderer.h"

namespace Dwarf
{
  std::shared_ptr<IRenderingPipeline>
  RenderingPipelineFactory::Create(PipelineType type)
  {
    switch (type)
    {
      case PipelineType::Forward: return std::make_shared<ForwardRenderer>();
      // case PipelineType::Deferred: return
      // std::make_shared<DeferredRenderer>();
      default: return nullptr;
    }
  }
} // namespace Dwarf