#include "DrawCallList.h"

namespace Dwarf
{
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
    {
      std::lock_guard<std::mutex> lock(m_DrawCallMutex);
      return m_DrawCalls;
    }
  }
}