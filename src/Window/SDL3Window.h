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

    /**
     * @brief Running logic for the beginning of a frame
     *
     */
    void
    NewFrame() override;

    /**
     * @brief Running logic for the end of a frame
     *
     */
    void
    EndFrame() override;

    /**
     * @brief Gets the width of the window
     *
     * @return Width in pixels
     */
    [[nodiscard]] auto
    GetWidth() const -> uint32_t override
    {
      return mData.Width;
    }

    /**
     * @brief Gets the height of the window
     *
     * @return Height in pixels
     */
    [[nodiscard]] auto
    GetHeight() const -> uint32_t override
    {
      return mData.Height;
    }

    /**
     * @brief Shows the window
     *
     */
    void
    ShowWindow() override;

    /**
     * @brief Hides the window
     *
     */
    void
    HideWindow() override;

    /**
     * @brief Enables/Disables VSync
     *
     * @param enabled Desired VSync state
     */
    void
    SetVSync(bool enabled) override;

    /**
     * @brief Checks if VSync is enabled window based
     *
     * @return true Enable VSync
     * @return false Disable VSync
     */
    auto
    IsVSync() -> bool override;

    /**
     * @brief Checks the close signal of the window (E.g. pressing the close
     * button in the window title bar)
     *
     * @return true Window should close
     * @return false Window does not have a closing signal
     */
    auto
    ShouldClose() -> bool override;

    /**
     * @brief Sets the title of the window
     *
     * @param windowTitle Window title
     */
    void
    SetWindowTitle(std::string_view windowTitle) override;

    /**
     * @brief Gets the underlying SDL window pointer
     *
     * @return The raw SDL window pointer
     */
    [[nodiscard]] auto
    GetNativeWindow() const -> SDL_Window* override
    {
      return mWindow;
    }

    /**
     * @brief Maximizes the window
     *
     */
    void
    MaximizeWindow() override;

    /**
     * @brief Checks if the window is in the maximized staet
     *
     * @return true If it is maximized
     * @return false It's not
     */
    auto
    IsWindowMaximized() -> bool override;

    /**
     * @brief Sets if the window should be in maximized state when showing it
     *
     * @param maximized Show window in maximized mode if true
     */
    void
    SetShowWindowMaximized(bool maximized) override;
  };
}