#pragma once

#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "IRenderingPipelineFactory.h"

namespace Dwarf
{
  class RenderingPipelineFactory : public IRenderingPipelineFactory
  {
  private:
    std::shared_ptr<IRendererApi> m_RendererApi;

  public:
    RenderingPipelineFactory(std::shared_ptr<IRendererApi> rendererApi);
    virtual ~RenderingPipelineFactory() = default;

    std::shared_ptr<IRenderingPipeline>
    Create(PipelineType type) override;
  };
} // namespace Dwarf