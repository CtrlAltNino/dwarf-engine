#pragma once

namespace Dwarf
{
  class IGraphicsContext
  {
  public:
    virtual ~IGraphicsContext() = default;

    virtual void
    Init() = 0;

    virtual void
    SwapBuffers() = 0;
  };
}