#pragma once

#include "Core/Rendering/DrawCall/IDrawCall.h"
#include <mutex>
namespace Dwarf
{
  class IDrawCallList
  {
  public:
    virtual ~IDrawCallList() = default;

    virtual void
    SubmitDrawCalls(std::vector<std::unique_ptr<IDrawCall>> drawCalls) = 0;

    virtual std::vector<std::unique_ptr<IDrawCall>>&
    GetDrawCalls() = 0;

    virtual std::mutex&
    GetMutex() = 0;

    virtual void
    Clear() = 0;
  };
}