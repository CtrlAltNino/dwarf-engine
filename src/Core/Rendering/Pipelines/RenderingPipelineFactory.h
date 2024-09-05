#pragma once

#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Core/Rendering/RendererApi/IRendererApiFactory.h"
#include "IRenderingPipelineFactory.h"

namespace Dwarf
{
  class RenderingPipelineFactory : public IRenderingPipelineFactory
  {
  private:
    std::shared_ptr<IRendererApi> m_RendererApi;

  public:
    RenderingPipelineFactory(
      std::shared_ptr<IRendererApiFactory> rendererApiFactory);
    virtual ~RenderingPipelineFactory() = default;

    std::shared_ptr<IRenderingPipeline>
    Create(PipelineType type) override;
  };
} // namespace Dwarf