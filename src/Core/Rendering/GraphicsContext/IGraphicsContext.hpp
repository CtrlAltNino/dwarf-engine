#pragma once

namespace Dwarf
{
  /**
   * @brief A class that represents an instance of a graphics api context
   *
   */
  class IGraphicsContext
  {
  public:
    virtual ~IGraphicsContext() = default;

    /**
     * @brief Initializes the the graphics context
     *
     */
    virtual void
    Init() = 0;

    /**
     * @brief Swaps the buffers
     *
     */
    virtual void
    SwapBuffers() = 0;
  };
}