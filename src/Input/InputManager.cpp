#include "Input/InputManager.h"
#include <imgui_impl_sdl3.h>

namespace Dwarf
{

  // glm::ivec2 InputManager::mCurrentMousePos = glm::ivec2(0);
  // glm::ivec2 InputManager::mLastMousePos = glm::ivec2(0);
  // glm::ivec2 InputManager::mDeltaMousePos = glm::ivec2(0);
  // glm::ivec2 InputManager::mDeltaScroll = glm::ivec2(0);

  // std::map<MOUSE_BUTTON, int> InputManager::mMouseButtonStates =
  //   std::map<MOUSE_BUTTON, int>();

  std::map<SDL_Scancode, KEYCODE> InputManager::s_KeyCodeMap = {
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

  // std::set<KEYCODE> InputManager::mKeysDown;
  // std::set<KEYCODE> InputManager::mKeysRepeat;
  // std::set<KEYCODE> InputManager::mKeysUp;

  std::map<MOUSE_BUTTON, int> InputManager::s_MouseCodeMap = {
    { MOUSE_BUTTON::LEFT, 1 },
    { MOUSE_BUTTON::RIGHT, 3 },
    { MOUSE_BUTTON::MIDDLE, 2 },
    { MOUSE_BUTTON::MOUSE_BUTTON_4, 4 },
    { MOUSE_BUTTON::MOUSE_BUTTON_5, 5 }
  };

  InputManager::InputManager(std::shared_ptr<IDwarfLogger> logger)
    : mLogger(logger)
  {
    mLogger->LogDebug(Log("InputManager created.", "InputManager"));
  }

  InputManager::~InputManager()
  {
    mLogger->LogDebug(Log("InputManager destroyed.", "InputManager"));
  }

  bool
  InputManager::GetKey(KEYCODE key) const
  {
    return mKeysDown.contains(key) || mKeysRepeat.contains(key);
  }

  bool
  InputManager::GetKeyDown(KEYCODE key) const
  {
    return mKeysDown.contains(key);
  }

  bool
  InputManager::GetKeyUp(KEYCODE key) const
  {
    return mKeysUp.contains(key);
  }

  bool
  InputManager::GetMouseButton(MOUSE_BUTTON button) const
  {
    return mMouseButtonStates.at(button) > 0;
  }

  bool
  InputManager::GetMouseButtonDown(MOUSE_BUTTON button) const
  {
    return mMouseButtonStates.at(button) == 1;
  }

  bool
  InputManager::GetMouseButtonUp(MOUSE_BUTTON button) const
  {
    return mMouseButtonStates.at(button) == 0;
  }

  void
  InputManager::SetMouseVisibility(bool visibilityState)
  {
    ImGui::SetMouseCursor(visibilityState ? ImGuiMouseCursor_Arrow
                                          : ImGuiMouseCursor_None);
  }

  glm::vec2
  InputManager::GetMousePosition() const
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

  glm::vec2
  InputManager::GetMouseDelta() const
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
      if (mouseButtonMask & SDL_BUTTON_MASK(s_MouseCodeMap[mCode]))
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
    KEYCODE keycode = s_KeyCodeMap[key];

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
    KEYCODE keycode = s_KeyCodeMap[key];

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

  glm::vec2
  InputManager::GetMouseScrollDelta() const
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