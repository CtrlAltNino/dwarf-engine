#pragma once

#include "Core/Rendering/Pipelines/IRenderingPipeline.hpp"

namespace Dwarf
{
  class IRenderingPipelineFactory
  {
    /**
     * @brief A class that creates rendering pipelines
     *
     */
  public:
    virtual ~IRenderingPipelineFactory() = default;

    /**
     * @brief Creates a Rendering Pipeline
     *
     * @return Unique pointer to the created rendering pipeline instance
     */
    [[nodiscard]] virtual auto
    Create() const -> std::unique_ptr<IRenderingPipeline> = 0;
  };
}