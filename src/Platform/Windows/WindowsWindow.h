#pragma once

#include "Core/Rendering/GraphicsContext/IGraphicsContextFactory.h"
#include "Window/IWindow.h"
#include "Core/Rendering/GraphicsContext/IGraphicsContext.h"
#include "UI/ImGuiLayer.h"

namespace Dwarf
{
  class WindowsWindow : public IWindow
  {
  public:
    explicit WindowsWindow(
      const WindowProps&                       props,
      std::shared_ptr<IGraphicsContextFactory> contextFactory);
    ~WindowsWindow() override;

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
    SetWindowTitle(std::string_view windowTitle) override;

    SDL_Window*
    GetNativeWindow() const override
    {
      return m_Window;
    }

    void
    MaximizeWindow() override;

  private:
    void
    Init(const WindowProps& props);

    SDL_Window*                              m_Window;
    std::shared_ptr<IGraphicsContext>        m_Context;
    std::shared_ptr<ImGuiLayer>              m_ImguiLayer;
    std::shared_ptr<IGraphicsContextFactory> m_ContextFactory;

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