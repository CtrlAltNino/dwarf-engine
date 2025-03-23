#pragma once

#include "Core/Rendering/GraphicsContext/IGraphicsContext.h"
#include "Logging/IDwarfLogger.h"
#include <SDL3/SDL.h>

namespace Dwarf
{
  class OpenGLContext : public IGraphicsContext
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    SDL_Window*                   mWindowHandle;
    SDL_GLContext                 mContext = nullptr;

  public:
    explicit OpenGLContext(std::shared_ptr<IDwarfLogger> logger,
                           SDL_Window*                   windowHandle);
    ~OpenGLContext() override;

    /**
     * @brief Initializes the the graphics context
     *
     */
    void
    Init() override;

    /**
     * @brief Swaps the buffers
     *
     */
    void
    SwapBuffers() override;
  };
}