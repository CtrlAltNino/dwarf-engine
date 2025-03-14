#pragma once

#include "Core/Rendering/GraphicsContext/IGraphicsContext.h"
#include "Logging/IDwarfLogger.h"
#include <SDL3/SDL.h>

namespace Dwarf
{
  class OpenGLContext : public IGraphicsContext
  {
  public:
    explicit OpenGLContext(std::shared_ptr<IDwarfLogger> logger,
                           SDL_Window*                   windowHandle);
    ~OpenGLContext() override;

    void
    Init() override;
    void
    SwapBuffers() override;

  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    SDL_Window*                   m_WindowHandle;
    SDL_GLContext                 m_Context;
  };
}