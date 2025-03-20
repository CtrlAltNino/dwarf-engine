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