#pragma once

#include "Core/Scene/Components/SceneComponents.h"
#include "IDrawCall.h"

namespace Dwarf
{
  /**
   * @brief A class that creates draw calls
   *
   */
  class IDrawCallFactory
  {
  public:
    virtual ~IDrawCallFactory() = default;

    /**
     * @brief Creates a draw call from the needed data.
     *
     * @param mesh The mesh data to use for the draw call
     * @param material The material to use for the draw call
     * @param transform The transform to use for the draw call
     * @return Unique pointer to the created draw call
     */
    virtual auto
    Create(std::unique_ptr<IMesh>& mesh,
           MaterialAsset&          material,
           TransformComponent&     transform) -> std::unique_ptr<IDrawCall> = 0;
  };
}