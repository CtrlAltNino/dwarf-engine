#pragma once

#include "pch.h"
#include <SDL2/SDL.h>

namespace Dwarf
{
#define KEYCODE_INITIALIZER { W, A, S, D, E, Q, R, LEFT_SHIFT, LEFT_CONTROL }
#define MOUSE_BUTTON_INITIALIZER                                               \
  { LEFT, RIGHT, MIDDLE, MOUSE_BUTTON_4, MOUSE_BUTTON_5 }
  enum class KEYCODE      KEYCODE_INITIALIZER;
  enum class MOUSE_BUTTON MOUSE_BUTTON_INITIALIZER;

  class IInputManager
  {
  public:
    virtual ~IInputManager() = default;

    virtual bool
    GetKey(KEYCODE key) const = 0;
    virtual bool
    GetKeyDown(KEYCODE key) const = 0;
    virtual bool
    GetKeyUp(KEYCODE key) const = 0;

    virtual bool
    GetMouseButton(MOUSE_BUTTON button) const = 0;
    virtual bool
    GetMouseButtonDown(MOUSE_BUTTON button) const = 0;
    virtual bool
    GetMouseButtonUp(MOUSE_BUTTON button) const = 0;

    virtual void
    SetMouseVisibility(bool visibilityState) = 0;

    virtual glm::vec2
    GetMousePosition() const = 0;
    virtual glm::vec2
    GetMouseDelta() const = 0;
    virtual glm::vec2
    GetMouseScrollDelta() const = 0;

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