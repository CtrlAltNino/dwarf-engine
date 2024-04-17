#pragma once

// system
#include <iostream>

// SDL
#include <SDL2/SDL.h>

#include "Window/Window.h"
#include "Core/Rendering/GraphicsContext.h"
#include "Core/UI/ImGuiLayer.h"

namespace Dwarf
{

  class OSXWindow : public Window
  {
  public:
    explicit OSXWindow(const WindowProps& props);
    ~OSXWindow() override;

    void
    NewFrame() override;
    void
    EndFrame() override;

    unsigned int
    GetWidth() const override
    {
      return m_Data.Width;
    }
    unsigned int
    GetHeight() const override
    {
      return m_Data.Height;
    }

    void
    ShowWindow() override;
    void
    HideWindow() override;

    void
    SetVSync(bool enabled) override;
    bool
    IsVSync() override;

    bool
    ShouldClose() override;

    void
    SetWindowTitle(std::string windowTitle) override;

    void*
    GetNativeWindow() const override
    {
      return m_Window;
    }

    GraphicsApi
    GetApi() override;

    void
    MaximizeWindow() override;

  private:
    virtual void
    Init(const WindowProps& props);
    virtual void
    Shutdown();

  private:
    SDL_Window*            m_Window;
    Scope<GraphicsContext> m_Context;
    GraphicsApi            m_Api;
    Ref<ImGuiLayer>        m_ImguiLayer;

    struct WindowData
    {
      std::string  Title;
      unsigned int Width;
      unsigned int Height;
      bool         VSync;
      bool         ShouldClose;
    };

    WindowData m_Data;
  };
}