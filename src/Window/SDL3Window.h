#pragma once

#include "Core/Rendering/GraphicsContext/IGraphicsContext.h"
#include "Core/Rendering/GraphicsContext/IGraphicsContextFactory.h"
#include "Editor/Stats/IEditorStats.h"
#include "Input/IInputManager.h"
#include "Logging/IDwarfLogger.h"
#include "UI/IImGuiLayer.h"
#include "UI/IImGuiLayerFactory.h"
#include "Window/IWindow.h"

namespace Dwarf
{
  class SDL3Window : public IWindow
  {
    struct WindowData
    {
      std::string Title;
      int         Width = 0;
      int         Height = 0;
      bool        VSync = false;
      bool        ShouldClose = false;
      bool        ShowMaximized = false;
    };

  private:
    SDL_Window*                              m_Window;
    std::unique_ptr<IGraphicsContext>        m_Context;
    std::unique_ptr<IImGuiLayer>             m_ImGuiLayer;
    std::shared_ptr<IGraphicsContextFactory> m_ContextFactory;
    std::shared_ptr<IImGuiLayerFactory>      m_ImguiLayerFactory;
    std::shared_ptr<IInputManager>           m_InputManager;
    std::shared_ptr<IDwarfLogger>            m_Logger;
    std::shared_ptr<IEditorStats>            m_EditorStats;

    WindowData m_Data;

  public:
    explicit SDL3Window(const WindowProps&                       props,
                        std::shared_ptr<IGraphicsContextFactory> contextFactory,
                        std::shared_ptr<IImGuiLayerFactory> imguiLayerFactory,
                        std::shared_ptr<IInputManager>      inputManager,
                        std::shared_ptr<IDwarfLogger>       logger,
                        std::shared_ptr<IEditorStats>       editorStats);
    ~SDL3Window() override;

    void
    newFrame() override;
    void
    endFrame() override;

    auto
    getWidth() const -> unsigned int override
    {
      return m_Data.Width;
    }

    auto
    getHeight() const -> unsigned int override
    {
      return m_Data.Height;
    }

    void
    showWindow() override;
    void
    hideWindow() override;

    void
    setVSync(bool enabled) override;
    auto
    isVSync() -> bool override;

    auto
    shouldClose() -> bool override;

    void
    setWindowTitle(std::string_view windowTitle) override;

    [[nodiscard]] auto
    getNativeWindow() const -> SDL_Window* override
    {
      return m_Window;
    }

    void
    maximizeWindow() override;

    auto
    isWindowMaximized() -> bool override;

    void
    setShowWindowMaximized(bool maximized) override;
  };
}