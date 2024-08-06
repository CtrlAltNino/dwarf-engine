#pragma once

#include "Core/Rendering/Pipelines/IRenderingPipeline.h"

namespace Dwarf
{
  class IRenderingPipelineFactory
  {
  public:
    virtual ~IRenderingPipelineFactory() = default;
    virtual std::shared_ptr<IRenderingPipeline>
    Create(PipelineType type) = 0;
  };
}