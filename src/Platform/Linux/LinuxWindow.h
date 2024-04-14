#pragma once

// system
#include <iostream>

// SDL
#include <SDL2/SDL.h>

#include "Window/Window.h"
#include "Core/Rendering/GraphicsContext.h"
#include "Core/UI/ImGuiLayer.h"

namespace Dwarf {

  class LinuxWindow : public Window
  {
  public:
    explicit LinuxWindow(GraphicsApi api);
    ~LinuxWindow() override;

    void NewFrame() override;
    void EndFrame() override;

    unsigned int GetWidth() const override { return m_Data.Width; }
    unsigned int GetHeight() const override { return m_Data.Height; }

    void ShowWindow() override;
    void HideWindow() override;

    // Window attributes
    void SetVSync(bool enabled) override;
    bool IsVSync() override;

    bool ShouldClose() override;

    void SetWindowTitle(std::string_view windowTitle) override;

    SDL_Window* GetNativeWindow() const override { return m_Window; }

    GraphicsApi GetApi() override;

    void MaximizeWindow() override;

  private:
    void Init(const WindowProps& props) override;
    SDL_Window* m_Window;
    Scope<GraphicsContext> m_Context;
    GraphicsApi m_Api;
    Ref<ImGuiLayer> m_ImguiLayer;

    struct WindowData
    {
      std::string Title;
      unsigned int Width;
      unsigned int Height;
      bool VSync;
      bool ShouldClose;
    };

    WindowData m_Data;
  };
}