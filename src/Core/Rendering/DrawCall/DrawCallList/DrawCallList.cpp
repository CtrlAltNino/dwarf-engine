#include "pch.hpp"

#include "DrawCallList.hpp"

namespace Dwarf
{
  DrawCallList::DrawCallList(std::shared_ptr<IDwarfLogger> logger)
    : mLogger(std::move(logger))
  {
  }

  DrawCallList::~DrawCallList()
  {
    std::lock_guard<std::mutex> lock(mDrawCallMutex);
    mDrawCalls.clear();
  }

  void
  DrawCallList::SubmitDrawCalls(
    std::vector<std::shared_ptr<IDrawCall>> drawCalls)
  {
    std::lock_guard<std::mutex> lock(mDrawCallMutex);
    mDrawCalls = std::move(drawCalls);
    mAllDrawCallsLoaded.store(false);

    mStats.DrawCallCount.store(mDrawCalls.size());
  }

  auto
  DrawCallList::GetDrawCalls() -> std::vector<std::shared_ptr<IDrawCall>>&
  {
    return mDrawCalls;
  }

  auto
  DrawCallList::GetMutex() -> std::mutex&
  {
    return mDrawCallMutex;
  }

  void
  DrawCallList::Clear()
  {
    mLogger->LogDebug(Log("Clearing Draw Calls", "DrawCallList"));
    std::lock_guard<std::mutex> lock(mDrawCallMutex);
    mDrawCalls.clear();
  }

  auto
  DrawCallList::GetStats() -> const DrawCallStatistics&
  {
    if (!mAllDrawCallsLoaded.load())
    {
      std::lock_guard<std::mutex> lock(mDrawCallMutex);
      uint32_t                    verts = 0;
      uint32_t                    indices = 0;
      uint32_t                    loaded = 0;
      for (const auto& drawCall : mDrawCalls)
      {
        if (drawCall->GetMeshBuffer() != nullptr)
        {
          verts += drawCall->GetMeshBuffer()->GetVertexCount();
          indices += drawCall->GetMeshBuffer()->GetIndexCount();
          loaded++;
        }
      }

      mStats.VertexCount.store(verts);
      mStats.TriangleCount.store(indices / 3);
      mAllDrawCallsLoaded = mDrawCalls.size() == loaded;
    }
    return mStats;
  }
}