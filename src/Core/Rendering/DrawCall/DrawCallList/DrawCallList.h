#pragma once

#include "Core/Rendering/DrawCall/IDrawCall.h"
#include "IDrawCallList.h"
#include <mutex>

namespace Dwarf
{
  class DrawCallList : public IDrawCallList
  {
  private:
    std::mutex                              mDrawCallMutex;
    std::vector<std::unique_ptr<IDrawCall>> mDrawCalls;

  public:
    ~DrawCallList() override;

    /**
     * @brief Store a new list of draw calls
     *
     * @param drawCalls Vector containing a list of draw calls
     */
    void
    SubmitDrawCalls(std::vector<std::unique_ptr<IDrawCall>> drawCalls) override;

    /**
     * @brief Retrieves the list of the draw calls
     *
     * @return Reference to the vector of the draw calls
     */
    auto
    GetDrawCalls() -> std::vector<std::unique_ptr<IDrawCall>>& override;

    /**
     * @brief Retrieves the mutex used for accessing the draw calls
     *
     * @return std::mutex&
     */
    auto
    GetMutex() -> std::mutex& override;

    /**
     * @brief Clears the draw call list
     *
     */
    void
    Clear() override;
  };
}