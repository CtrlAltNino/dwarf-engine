#pragma once
#include "pch.h"
#include "Core/Base.h"
#include "SDL2/SDL.h"

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