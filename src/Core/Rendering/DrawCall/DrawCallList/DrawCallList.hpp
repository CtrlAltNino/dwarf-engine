#pragma once

#include "Core/Rendering/DrawCall/IDrawCall.hpp"
#include "IDrawCallList.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <memory>
#include <mutex>

namespace Dwarf
{
  class DrawCallList : public IDrawCallList
  {
  private:
    std::shared_ptr<IDwarfLogger>           mLogger;
    std::mutex                              mDrawCallMutex;
    std::vector<std::shared_ptr<IDrawCall>> mDrawCalls;
    DrawCallStatistics                      mStats;
    std::atomic<bool>                       mAllDrawCallsLoaded = false;

  public:
    DrawCallList(std::shared_ptr<IDwarfLogger> logger);
    ~DrawCallList() override;

    /**
     * @brief Store a new list of draw calls
     *
     * @param drawCalls Vector containing a list of draw calls
     */
    void
    SubmitDrawCalls(std::vector<std::shared_ptr<IDrawCall>> drawCalls) override;

    /**
     * @brief Retrieves the list of the draw calls
     *
     * @return Reference to the vector of the draw calls
     */
    auto
    GetDrawCalls() -> std::vector<std::shared_ptr<IDrawCall>>& override;

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

    auto
    GetStats() -> const DrawCallStatistics& override;
  };
}