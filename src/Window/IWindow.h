#pragma once
#include "Core/Base.h"
#include <SDL3/SDL.h>

#include <utility>

namespace Dwarf
{
  // TODO: This struct looks like sh*t
  struct WindowProps
  {
    std::string Title = "Dwarf Engine";
    int         Width = 0;
    int         Height = 0;
    GraphicsApi Api = GraphicsApi::OpenGL;
    bool        Maximized = false;
  };

  class IWindow
  {
  public:
    virtual ~IWindow() = default;

    virtual void
    newFrame() = 0;
    virtual void
    endFrame() = 0;

    virtual auto
    getWidth() const -> uint32_t = 0;
    virtual auto
    getHeight() const -> uint32_t = 0;

    virtual void
    setVSync(bool enabled) = 0;
    virtual auto
    isVSync() -> bool = 0;

    [[nodiscard]] virtual auto
    getNativeWindow() const -> SDL_Window* = 0;

    virtual void
    showWindow() = 0;
    virtual void
    hideWindow() = 0;

    virtual auto
    shouldClose() -> bool = 0;

    virtual void
    maximizeWindow() = 0;

    virtual void
    setWindowTitle(std::string_view windowTitle) = 0;

    virtual auto
    isWindowMaximized() -> bool = 0;

    virtual void
    setShowWindowMaximized(bool maximized) = 0;
  };
}