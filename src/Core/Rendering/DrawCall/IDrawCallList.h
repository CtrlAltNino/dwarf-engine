#pragma once

#include "Core/Rendering/DrawCall/IDrawCall.h"
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
  };
}