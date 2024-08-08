#pragma once

#include "Core/Base.h"
#include <SDL2/SDL.h>

namespace Dwarf
{
  class IImGuiLayer
  {
  protected:
    SDL_Window* m_Window;

  public:
    virtual ~IImGuiLayer() = default;

    virtual void
    OnAttach(SDL_Window* window) = 0;

    virtual void
    OnDetach() = 0;

    virtual void
    Begin() = 0;

    virtual void
    End() = 0;

    virtual void
    HandleSDLEvent(SDL_Event* event) = 0;
  };
}