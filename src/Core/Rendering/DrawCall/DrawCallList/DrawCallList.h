#pragma once

#include "Core/Rendering/DrawCall/IDrawCall.h"
#include "IDrawCallList.h"
#include <mutex>
namespace Dwarf
{
  class DrawCallList : public IDrawCallList
  {
  private:
    std::mutex                              m_DrawCallMutex;
    std::vector<std::unique_ptr<IDrawCall>> m_DrawCalls;

  public:
    ~DrawCallList() override;

    void
    SubmitDrawCalls(std::vector<std::unique_ptr<IDrawCall>> drawCalls) override;

    std::vector<std::unique_ptr<IDrawCall>>&
    GetDrawCalls() override;

    std::mutex&
    GetMutex() override;

    void
    Clear() override;
  };
}