#pragma once

#include "Logging/IDwarfLogger.h"
#include "Window/IWindow.h"
#include "Core/Rendering/GraphicsContext/IGraphicsContextFactory.h"
#include "UI/IImGuiLayerFactory.h"
#include "Core/Rendering/GraphicsContext/IGraphicsContext.h"
#include "UI/IImGuiLayer.h"
#include "Input/IInputManager.h"

namespace Dwarf
{
  class LinuxWindow : public IWindow
  {
  public:
    explicit LinuxWindow(
      const WindowProps&                       props,
      std::shared_ptr<IGraphicsContextFactory> contextFactory,
      std::shared_ptr<IImGuiLayerFactory>      imguiLayerFactory,
      std::shared_ptr<IInputManager>           inputManager,
      std::shared_ptr<IDwarfLogger>            logger);
    ~LinuxWindow() override;

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
    std::shared_ptr<IImGuiLayer>             m_ImGuiLayer;
    std::shared_ptr<IGraphicsContextFactory> m_ContextFactory;
    std::shared_ptr<IImGuiLayerFactory>      m_ImguiLayerFactory;
    std::shared_ptr<IInputManager>           m_InputManager;
    std::shared_ptr<IDwarfLogger>            m_Logger;

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