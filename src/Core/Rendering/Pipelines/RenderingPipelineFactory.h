#pragma once

#include "IRenderingPipelineFactory.h"

namespace Dwarf
{
  class RenderingPipelineFactory : public IRenderingPipelineFactory
  {
  public:
    RenderingPipelineFactory() = default;
    virtual ~RenderingPipelineFactory() = default;

    std::shared_ptr<IRenderingPipeline>
    Create(PipelineType type) override;
  };
} // namespace Dwarf