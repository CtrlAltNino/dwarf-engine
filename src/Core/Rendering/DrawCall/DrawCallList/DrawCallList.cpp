#include "DrawCallList.h"

namespace Dwarf
{
  DrawCallList::~DrawCallList()
  {
    {
      std::lock_guard<std::mutex> lock(mDrawCallMutex);
      mDrawCalls.clear();
    }
  }

  void
  DrawCallList::SubmitDrawCalls(
    std::vector<std::unique_ptr<IDrawCall>> drawCalls)
  {
    {
      std::lock_guard<std::mutex> lock(mDrawCallMutex);
      mDrawCalls = std::move(drawCalls);
    }
  }

  std::vector<std::unique_ptr<IDrawCall>>&
  DrawCallList::GetDrawCalls()
  {
    return mDrawCalls;
  }

  std::mutex&
  DrawCallList::GetMutex()
  {
    return mDrawCallMutex;
  }

  void
  DrawCallList::Clear()
  {
    {
      std::lock_guard<std::mutex> lock(mDrawCallMutex);
      mDrawCalls.clear();
    }
  }
}