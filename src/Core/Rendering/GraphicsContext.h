#pragma once

#include "Core/Base.h"
#include "SDL2/SDL.h"

namespace Dwarf
{

  class GraphicsContext
  {
  public:
    virtual ~GraphicsContext() = default;

    virtual void
    Init() = 0;
    virtual void
    SwapBuffers() = 0;

    static Scope<GraphicsContext>
    Create(SDL_Window* window);
  };
}