#pragma once

#include "Editor/Stats/IEditorStats.h"
#include "Logging/IDwarfLogger.h"
#include "Window/IWindow.h"
#include "Core/Rendering/GraphicsContext/IGraphicsContextFactory.h"
#include "UI/IImGuiLayerFactory.h"
#include "Core/Rendering/GraphicsContext/IGraphicsContext.h"
#include "UI/IImGuiLayer.h"
#include "Input/IInputManager.h"

namespace Dwarf
{
  class WindowsWindow : public IWindow
  {
  public:
    explicit WindowsWindow(const WindowProps&            props,
                           IGraphicsContextFactory&      contextFactory,
                           IImGuiLayerFactory&           imguiLayerFactory,
                           IInputManager&                inputManager,
                           std::shared_ptr<IDwarfLogger> logger,
                           std::shared_ptr<IEditorStats> editorStats);
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

    bool
    IsWindowMaximized() override;

  private:
    void
    Init(const WindowProps& props);

    SDL_Window*                       m_Window;
    std::unique_ptr<IGraphicsContext> m_Context;
    std::unique_ptr<IImGuiLayer>      m_ImGuiLayer;
    IGraphicsContextFactory&          m_ContextFactory;
    IImGuiLayerFactory&               m_ImguiLayerFactory;
    IInputManager&                    m_InputManager;
    std::shared_ptr<IDwarfLogger>     m_Logger;
    std::shared_ptr<IEditorStats>     m_EditorStats;

    struct WindowData
    {
      std::string  Title = "";
      unsigned int Width = 0;
      unsigned int Height = 0;
      bool         VSync = false;
      bool         ShouldClose = false;
    };

    WindowData m_Data;
  };
}