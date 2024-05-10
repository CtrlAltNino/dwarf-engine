#pragma once
#include "pch.h"
#include "Core/Base.h"
#include <SDL2/SDL.h>

namespace Dwarf
{
  struct WindowProps
  {
    std::string Title;
    uint32_t    Width;
    uint32_t    Height;
    GraphicsApi Api;

    WindowProps(const std::string& title = "Dwarf Engine",
                uint32_t           width = 1600,
                uint32_t           height = 900)
      : Title(title)
      , Width(width)
      , Height(height)
    {
    }
  };

  class Window
  {
  public:
    virtual ~Window() = default;

    virtual void
    NewFrame() = 0;
    virtual void
    EndFrame() = 0;

    virtual uint32_t
    GetWidth() const = 0;
    virtual uint32_t
    GetHeight() const = 0;

    virtual void
    SetVSync(bool enabled) = 0;
    virtual bool
    IsVSync() = 0;

    virtual SDL_Window*
    GetNativeWindow() const = 0;

    virtual GraphicsApi
    GetApi() = 0;

    virtual void
    ShowWindow() = 0;
    virtual void
    HideWindow() = 0;

    virtual bool
    ShouldClose() = 0;

    virtual void
    MaximizeWindow() = 0;

    virtual void
    SetWindowTitle(std::string_view windowTitle) = 0;

    static std::unique_ptr<Window>
    Create(const WindowProps& props = WindowProps());
  };
}