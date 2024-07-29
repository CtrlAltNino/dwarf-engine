#pragma once

#include "Core/Rendering/GraphicsContext/IGraphicsContext.h"
#include <SDL2/SDL.h>

namespace Dwarf
{
  class OpenGLContext : public IGraphicsContext
  {
  public:
    explicit OpenGLContext(SDL_Window* windowHandle);
    ~OpenGLContext() override;

    void
    Init() override;
    void
    SwapBuffers() override;

  private:
    SDL_Window*   m_WindowHandle;
    SDL_GLContext m_Context;
  };
}