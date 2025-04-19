#include "pch.hpp"

#include "Input/InputManager.hpp"
#include <imgui_impl_sdl3.h>

namespace Dwarf
{
  std::map<SDL_Scancode, KEYCODE> InputManager::sKeyCodeMap = {
    { SDL_SCANCODE_W, KEYCODE::W },
    { SDL_SCANCODE_A, KEYCODE::A },
    { SDL_SCANCODE_S, KEYCODE::S },
    { SDL_SCANCODE_D, KEYCODE::D },
    { SDL_SCANCODE_E, KEYCODE::E },
    { SDL_SCANCODE_Q, KEYCODE::Q },
    { SDL_SCANCODE_R, KEYCODE::R },
    { SDL_SCANCODE_LSHIFT, KEYCODE::LEFT_SHIFT },
    { SDL_SCANCODE_LCTRL, KEYCODE::LEFT_CONTROL }
  };

  std::map<MOUSE_BUTTON, int> InputManager::sMouseCodeMap = {
    { MOUSE_BUTTON::LEFT, 1 },
    { MOUSE_BUTTON::RIGHT, 3 },
    { MOUSE_BUTTON::MIDDLE, 2 },
    { MOUSE_BUTTON::MOUSE_BUTTON_4, 4 },
    { MOUSE_BUTTON::MOUSE_BUTTON_5, 5 }
  };

  InputManager::InputManager(std::shared_ptr<IDwarfLogger> logger)
    : mLogger(std::move(logger))
  {
    mLogger->LogDebug(Log("InputManager created.", "InputManager"));
  }

  InputManager::~InputManager()
  {
    mLogger->LogDebug(Log("InputManager destroyed.", "InputManager"));
  }

  auto
  InputManager::GetKey(KEYCODE key) const -> bool
  {
    return mKeysDown.contains(key) || mKeysRepeat.contains(key);
  }

  auto
  InputManager::GetKeyDown(KEYCODE key) const -> bool
  {
    return mKeysDown.contains(key);
  }

  auto
  InputManager::GetKeyUp(KEYCODE key) const -> bool
  {
    return mKeysUp.contains(key);
  }

  auto
  InputManager::GetMouseButton(MOUSE_BUTTON button) const -> bool
  {
    return mMouseButtonStates.at(button) > 0;
  }

  auto
  InputManager::GetMouseButtonDown(MOUSE_BUTTON button) const -> bool
  {
    return mMouseButtonStates.at(button) == 1;
  }

  auto
  InputManager::GetMouseButtonUp(MOUSE_BUTTON button) const -> bool
  {
    return mMouseButtonStates.at(button) == 0;
  }

  auto
  InputManager::GetMousePosition() const -> glm::vec2
  {
    return mCurrentMousePos;
  }

  void
  InputManager::SetDeltaMousePos(float x, float y)
  {
    mDeltaMousePos = { x, y };
  }

  void
  InputManager::SetDeltaMouseScroll(float x, float y)
  {
    mDeltaScroll = { x, y };
  }

  auto
  InputManager::GetMouseDelta() const -> glm::vec2
  {
    return mDeltaMousePos;
  }

  void
  InputManager::OnUpdate()
  {
    using enum MOUSE_BUTTON;
    std::array<MOUSE_BUTTON, 5> mArr = MOUSE_BUTTON_INITIALIZER;
    mLastMousePos = mCurrentMousePos;
    Uint32 mouseButtonMask =
      SDL_GetMouseState(&mCurrentMousePos.x, &mCurrentMousePos.y);

    for (const MOUSE_BUTTON& mCode : mArr)
    {
      if (mouseButtonMask & SDL_BUTTON_MASK(sMouseCodeMap[mCode]))
      {
        // mousePressSet.insert(mCode);
        if (mMouseButtonStates[mCode] < 2)
        {
          mMouseButtonStates[mCode]++;
        }
      }
      else
      {
        mMouseButtonStates[mCode] = 0;
      }
    }

    SDL_GetRelativeMouseState(&mDeltaMousePos.x, &mDeltaMousePos.y);
  }

  void
  InputManager::ProcessKeyDown(SDL_Scancode key)
  {
    KEYCODE keycode = sKeyCodeMap[key];

    if (mKeysDown.contains(keycode))
    {
      mKeysDown.erase(keycode);
      mKeysRepeat.emplace(keycode);
    }
    else
    {
      mKeysDown.emplace(keycode);
    }
  }

  void
  InputManager::ProcessKeyUp(SDL_Scancode key)
  {
    KEYCODE keycode = sKeyCodeMap[key];

    if (mKeysDown.contains(keycode))
    {
      mKeysDown.erase(keycode);
    }

    if (mKeysRepeat.contains(keycode))
    {
      mKeysRepeat.erase(keycode);
    }

    mKeysUp.emplace(keycode);
  }

  auto
  InputManager::GetMouseScrollDelta() const -> glm::vec2
  {
    return mDeltaScroll;
  }

  void
  InputManager::ProcessScroll(SDL_Event const& event)
  {

    switch (event.wheel.type)
    {
      case SDL_EVENT_MOUSE_WHEEL:
        {
          mDeltaScroll = { event.wheel.x, event.wheel.y };
        }
        break;

      default: break;
    }
  }
}