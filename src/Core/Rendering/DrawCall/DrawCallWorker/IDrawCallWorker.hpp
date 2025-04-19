#pragma once

namespace Dwarf
{
  /**
   * @brief A class that constructs draw calls in it's own thread
   *
   */
  class IDrawCallWorker
  {
  public:
    virtual ~IDrawCallWorker() = default;

    /**
     * @brief Signals to rebuild the draw calls
     *
     */
    virtual void
    Invalidate() = 0;
  };
}