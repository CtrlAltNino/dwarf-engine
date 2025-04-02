#pragma once
#include "Core/Base.h"
#include <SDL3/SDL.h>

namespace Dwarf
{
  // TODO: This struct looks like sh*t
  struct WindowProps
  {
    std::string Title = "Dwarf Engine";
    int         Width = 0;
    int         Height = 0;
    GraphicsApi Api = GraphicsApi::OpenGL;
    bool        Resizable = false;
  };

  /**
   * @brief Class for abstracting window management
   *
   */
  class IWindow
  {
  public:
    virtual ~IWindow() = default;

    /**
     * @brief Running logic for the beginning of a frame
     *
     */
    virtual void
    NewFrame() = 0;

    /**
     * @brief Running logic for the end of a frame
     *
     */
    virtual void
    EndFrame() = 0;

    /**
     * @brief Gets the width of the window
     *
     * @return Width in pixels
     */
    [[nodiscard]] virtual auto
    GetWidth() const -> uint32_t = 0;

    /**
     * @brief Gets the height of the window
     *
     * @return Height in pixels
     */
    [[nodiscard]] virtual auto
    GetHeight() const -> uint32_t = 0;

    /**
     * @brief Enables/Disables VSync
     *
     * @param enabled Desired VSync state
     */
    virtual void
    SetVSync(bool enabled) = 0;

    /**
     * @brief Checks if VSync is enabled window based
     *
     * @return true Enable VSync
     * @return false Disable VSync
     */
    virtual auto
    IsVSync() -> bool = 0;

    /**
     * @brief Gets the underlying SDL window pointer
     *
     * @return The raw SDL window pointer
     */
    [[nodiscard]] virtual auto
    GetNativeWindow() const -> SDL_Window* = 0;

    /**
     * @brief Shows the window
     *
     */
    virtual void
    ShowWindow() = 0;

    /**
     * @brief Hides the window
     *
     */
    virtual void
    HideWindow() = 0;

    /**
     * @brief Checks the close signal of the window (E.g. pressing the close
     * button in the window title bar)
     *
     * @return true Window should close
     * @return false Window does not have a closing signal
     */
    virtual auto
    ShouldClose() -> bool = 0;

    /**
     * @brief Maximizes the window
     *
     */
    virtual void
    MaximizeWindow() = 0;

    /**
     * @brief Sets the title of the window
     *
     * @param windowTitle Window title
     */
    virtual void
    SetWindowTitle(std::string_view windowTitle) = 0;

    /**
     * @brief Checks if the window is in the maximized staet
     *
     * @return true If it is maximized
     * @return false It's not
     */
    virtual auto
    IsWindowMaximized() -> bool = 0;

    /**
     * @brief Sets if the window should be in maximized state when showing it
     *
     * @param maximized Show window in maximized mode if true
     */
    virtual void
    SetShowWindowMaximized(bool maximized) = 0;

    /**
     * @brief Sets the mouse visibility. While invisible the mouse will not
     * move, but the delta mouse position will still be reported.
     *
     * @param visibilityState
     */
    virtual void
    SetMouseVisibility(bool visibilityState) = 0;
  };
}