#pragma once

namespace Dwarf
{
  class IDrawCallWorker
  {
  public:
    virtual ~IDrawCallWorker() = default;

    virtual void
    Invalidate() = 0;
  };
}