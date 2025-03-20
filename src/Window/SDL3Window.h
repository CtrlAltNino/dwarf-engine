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
    SDL_Window*                              mWindow;
    std::unique_ptr<IGraphicsContext>        mContext;
    std::unique_ptr<IImGuiLayer>             mImGuiLayer;
    std::shared_ptr<IGraphicsContextFactory> mContextFactory;
    std::shared_ptr<IImGuiLayerFactory>      mImguiLayerFactory;
    std::shared_ptr<IInputManager>           mInputManager;
    std::shared_ptr<IDwarfLogger>            mLogger;
    std::shared_ptr<IEditorStats>            mEditorStats;

    WindowData mData;

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
      return mData.Width;
    }

    auto
    getHeight() const -> unsigned int override
    {
      return mData.Height;
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
      return mWindow;
    }

    void
    maximizeWindow() override;

    auto
    isWindowMaximized() -> bool override;

    void
    setShowWindowMaximized(bool maximized) override;
  };
}