#pragma once
#include "pch.h"
#include "Core/Base.h"
#include <SDL3/SDL.h>

namespace Dwarf
{
  // TODO: This struct looks like sh*t
  struct WindowProps
  {
    std::string Title = "";
    uint32_t    Width = 0;
    uint32_t    Height = 0;
    GraphicsApi Api = GraphicsApi::None;
    bool        Maximized = false;

    WindowProps(const std::string& title = "Dwarf Engine",
                uint32_t           width = 1600,
                uint32_t           height = 900,
                GraphicsApi        api = GraphicsApi::OpenGL)
      : Title(title)
      , Width(width)
      , Height(height)
      , Api(api)
    {
    }
  };

  class IWindow
  {
  public:
    virtual ~IWindow() = default;

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

    virtual bool
    IsWindowMaximized() = 0;

    virtual void
    SetShowWindowMaximized(bool maximized) = 0;
  };
}