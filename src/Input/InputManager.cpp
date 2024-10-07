#include "Input/InputManager.h"
#include <imgui_impl_sdl2.h>

namespace Dwarf
{

  // glm::ivec2 InputManager::m_CurrentMousePos = glm::ivec2(0);
  // glm::ivec2 InputManager::m_LastMousePos = glm::ivec2(0);
  // glm::ivec2 InputManager::m_DeltaMousePos = glm::ivec2(0);
  // glm::ivec2 InputManager::m_DeltaScroll = glm::ivec2(0);

  // std::map<MOUSE_BUTTON, int> InputManager::m_MouseButtonStates =
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

  // std::set<KEYCODE> InputManager::m_KeysDown;
  // std::set<KEYCODE> InputManager::m_KeysRepeat;
  // std::set<KEYCODE> InputManager::m_KeysUp;

  std::map<MOUSE_BUTTON, int> InputManager::s_MouseCodeMap = {
    { MOUSE_BUTTON::LEFT, 1 },
    { MOUSE_BUTTON::RIGHT, 3 },
    { MOUSE_BUTTON::MIDDLE, 2 },
    { MOUSE_BUTTON::MOUSE_BUTTON_4, 4 },
    { MOUSE_BUTTON::MOUSE_BUTTON_5, 5 }
  };

  InputManager::InputManager(std::shared_ptr<IDwarfLogger> logger)
    : m_Logger(logger)
  {
    m_Logger->LogDebug(Log("InputManager created.", "InputManager"));
  }

  InputManager::~InputManager()
  {
    m_Logger->LogDebug(Log("InputManager destroyed.", "InputManager"));
  }

  bool
  InputManager::GetKey(KEYCODE key) const
  {
    return m_KeysDown.contains(key) || m_KeysRepeat.contains(key);
  }

  bool
  InputManager::GetKeyDown(KEYCODE key) const
  {
    return m_KeysDown.contains(key);
  }

  bool
  InputManager::GetKeyUp(KEYCODE key) const
  {
    return m_KeysUp.contains(key);
  }

  bool
  InputManager::GetMouseButton(MOUSE_BUTTON button) const
  {
    return m_MouseButtonStates.at(button) > 0;
  }

  bool
  InputManager::GetMouseButtonDown(MOUSE_BUTTON button) const
  {
    return m_MouseButtonStates.at(button) == 1;
  }

  bool
  InputManager::GetMouseButtonUp(MOUSE_BUTTON button) const
  {
    return m_MouseButtonStates.at(button) == 0;
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
    return m_CurrentMousePos;
  }

  void
  InputManager::SetDeltaMousePos(float x, float y)
  {
    m_DeltaMousePos = { x, y };
  }

  void
  InputManager::SetDeltaMouseScroll(float x, float y)
  {
    m_DeltaScroll = { x, y };
  }

  glm::vec2
  InputManager::GetMouseDelta() const
  {
    return m_DeltaMousePos;
  }

  void
  InputManager::OnUpdate()
  {
    using enum MOUSE_BUTTON;
    std::array<MOUSE_BUTTON, 5> mArr = MOUSE_BUTTON_INITIALIZER;
    m_LastMousePos = m_CurrentMousePos;
    Uint32 mouseButtonMask =
      SDL_GetMouseState(&m_CurrentMousePos.x, &m_CurrentMousePos.y);

    for (const MOUSE_BUTTON& mCode : mArr)
    {
      if (mouseButtonMask & SDL_BUTTON(s_MouseCodeMap[mCode]))
      {
        // mousePressSet.insert(mCode);
        if (m_MouseButtonStates[mCode] < 2)
        {
          m_MouseButtonStates[mCode]++;
        }
      }
      else
      {
        m_MouseButtonStates[mCode] = 0;
      }
    }

    SDL_GetRelativeMouseState(&m_DeltaMousePos.x, &m_DeltaMousePos.y);
  }

  void
  InputManager::ProcessKeyDown(SDL_Scancode key)
  {
    KEYCODE keycode = s_KeyCodeMap[key];

    if (m_KeysDown.contains(keycode))
    {
      m_KeysDown.erase(keycode);
      m_KeysRepeat.emplace(keycode);
    }
    else
    {
      m_KeysDown.emplace(keycode);
    }
  }

  void
  InputManager::ProcessKeyUp(SDL_Scancode key)
  {
    KEYCODE keycode = s_KeyCodeMap[key];

    if (m_KeysDown.contains(keycode))
    {
      m_KeysDown.erase(keycode);
    }

    if (m_KeysRepeat.contains(keycode))
    {
      m_KeysRepeat.erase(keycode);
    }

    m_KeysUp.emplace(keycode);
  }

  glm::vec2
  InputManager::GetMouseScrollDelta() const
  {
    return m_DeltaScroll;
  }

  void
  InputManager::ProcessScroll(SDL_Event const& event)
  {

    switch (event.wheel.type)
    {
      case SDL_MOUSEWHEEL:
        {
          m_DeltaScroll = { event.wheel.x, event.wheel.y };
        }
        break;

      default: break;
    }
  }
}