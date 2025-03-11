#include "DrawCallList.h"

namespace Dwarf
{
  DrawCallList::~DrawCallList()
  {
    {
      std::lock_guard<std::mutex> lock(m_DrawCallMutex);
      m_DrawCalls.clear();
    }
  }

  void
  DrawCallList::SubmitDrawCalls(
    std::vector<std::unique_ptr<IDrawCall>> drawCalls)
  {
    {
      std::lock_guard<std::mutex> lock(m_DrawCallMutex);
      m_DrawCalls = std::move(drawCalls);
    }
  }

  std::vector<std::unique_ptr<IDrawCall>>&
  DrawCallList::GetDrawCalls()
  {
    return m_DrawCalls;
  }

  std::mutex&
  DrawCallList::GetMutex()
  {
    return m_DrawCallMutex;
  }

  void
  DrawCallList::Clear()
  {
    {
      std::lock_guard<std::mutex> lock(m_DrawCallMutex);
      m_DrawCalls.clear();
    }
  }
}