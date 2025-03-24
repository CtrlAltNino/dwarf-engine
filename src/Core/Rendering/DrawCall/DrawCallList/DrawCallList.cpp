#include "pch.h"

#include "DrawCallList.h"

namespace Dwarf
{
  DrawCallList::~DrawCallList()
  {
    std::lock_guard<std::mutex> lock(mDrawCallMutex);
    mDrawCalls.clear();
  }

  void
  DrawCallList::SubmitDrawCalls(
    std::vector<std::unique_ptr<IDrawCall>> drawCalls)
  {
    std::lock_guard<std::mutex> lock(mDrawCallMutex);
    mDrawCalls = std::move(drawCalls);
  }

  auto
  DrawCallList::GetDrawCalls() -> std::vector<std::unique_ptr<IDrawCall>>&
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
    std::lock_guard<std::mutex> lock(mDrawCallMutex);
    mDrawCalls.clear();
  }
}