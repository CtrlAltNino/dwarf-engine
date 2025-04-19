#pragma once

#include "Core/Rendering/DrawCall/IDrawCall.hpp"
#include <mutex>

namespace Dwarf
{
  struct DrawCallStatistics
  {
    std::atomic<uint32_t> DrawCallCount = 0;
    std::atomic<uint32_t> TriangleCount = 0;
    std::atomic<uint32_t> VertexCount = 0;
  };

  /**
   * @brief A class that contains a list of draw calls and allows for thread
   * safe access
   *
   */
  class IDrawCallList
  {
  public:
    virtual ~IDrawCallList() = default;

    /**
     * @brief Store a new list of draw calls
     *
     * @param drawCalls Vector containing a list of draw calls
     */
    virtual void
    SubmitDrawCalls(std::vector<std::unique_ptr<IDrawCall>> drawCalls) = 0;

    /**
     * @brief Retrieves the list of the draw calls
     *
     * @return Reference to the vector of the draw calls
     */
    virtual auto
    GetDrawCalls() -> std::vector<std::unique_ptr<IDrawCall>>& = 0;

    /**
     * @brief Retrieves the mutex used for accessing the draw calls
     *
     * @return std::mutex&
     */
    virtual auto
    GetMutex() -> std::mutex& = 0;

    /**
     * @brief Clears the draw call list
     *
     */
    virtual void
    Clear() = 0;

    [[nodiscard]] virtual auto
    GetStats() -> const DrawCallStatistics& = 0;
  };
}