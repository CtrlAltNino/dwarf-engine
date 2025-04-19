#pragma once

#include <SDL3/SDL.h>

namespace Dwarf
{
#define KEYCODE_INITIALIZER { W, A, S, D, E, Q, R, LEFT_SHIFT, LEFT_CONTROL }
#define MOUSE_BUTTON_INITIALIZER                                               \
  { LEFT, RIGHT, MIDDLE, MOUSE_BUTTON_4, MOUSE_BUTTON_5 }
  enum class KEYCODE : uint8_t      KEYCODE_INITIALIZER;
  enum class MOUSE_BUTTON : uint8_t MOUSE_BUTTON_INITIALIZER;

  class IInputManager
  {
  public:
    virtual ~IInputManager() = default;

    [[nodiscard]] virtual auto
    GetKey(KEYCODE key) const -> bool = 0;
    [[nodiscard]] virtual auto
    GetKeyDown(KEYCODE key) const -> bool = 0;
    [[nodiscard]] virtual auto
    GetKeyUp(KEYCODE key) const -> bool = 0;

    [[nodiscard]] virtual auto
    GetMouseButton(MOUSE_BUTTON button) const -> bool = 0;
    [[nodiscard]] virtual auto
    GetMouseButtonDown(MOUSE_BUTTON button) const -> bool = 0;
    [[nodiscard]] virtual auto
    GetMouseButtonUp(MOUSE_BUTTON button) const -> bool = 0;

    [[nodiscard]] virtual auto
    GetMousePosition() const -> glm::vec2 = 0;
    [[nodiscard]] virtual auto
    GetMouseDelta() const -> glm::vec2 = 0;
    [[nodiscard]] virtual auto
    GetMouseScrollDelta() const -> glm::vec2 = 0;

    virtual void
    SetDeltaMousePos(float x, float y) = 0;
    virtual void
    SetDeltaMouseScroll(float x, float y) = 0;

    virtual void
    ProcessKeyDown(SDL_Scancode keyCode) = 0;
    virtual void
    ProcessKeyUp(SDL_Scancode keyCode) = 0;
    virtual void
    ProcessScroll(SDL_Event const& event) = 0;

    virtual void
    OnUpdate() = 0;
  };
}