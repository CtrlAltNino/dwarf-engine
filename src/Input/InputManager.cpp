#include "dpch.h"
#include "Input/InputManager.h"

namespace Dwarf
{

    glm::ivec2 InputManager::s_CurrentMousePos = glm::ivec2(0);
    glm::ivec2 InputManager::s_LastMousePos = glm::ivec2(0);
    glm::ivec2 InputManager::s_DeltaMousePos = glm::ivec2(0);

    std::map<MOUSE_BUTTON, int> InputManager::s_MouseButtonStates = std::map<MOUSE_BUTTON, int>();

    std::map<SDL_Scancode, KEYCODE> InputManager::s_KeyCodeMap = {{SDL_SCANCODE_W, KEYCODE::W},
                                                                  {SDL_SCANCODE_A, KEYCODE::A},
                                                                  {SDL_SCANCODE_S, KEYCODE::S},
                                                                  {SDL_SCANCODE_D, KEYCODE::D},
                                                                  {SDL_SCANCODE_E, KEYCODE::E},
                                                                  {SDL_SCANCODE_Q, KEYCODE::Q},
                                                                  {SDL_SCANCODE_R, KEYCODE::R},
                                                                  {SDL_SCANCODE_LSHIFT, KEYCODE::LEFT_SHIFT},
                                                                  {SDL_SCANCODE_LCTRL, KEYCODE::LEFT_CONTROL}};

    std::set<KEYCODE> InputManager::s_KeysDown;
    std::set<KEYCODE> InputManager::s_KeysRepeat;
    std::set<KEYCODE> InputManager::s_KeysUp;

    std::map<MOUSE_BUTTON, int> InputManager::s_MouseCodeMap = {{MOUSE_BUTTON::LEFT, 1},
                                                                {MOUSE_BUTTON::RIGHT, 3},
                                                                {MOUSE_BUTTON::MIDDLE, 2},
                                                                {MOUSE_BUTTON::MOUSE_BUTTON_4, 4},
                                                                {MOUSE_BUTTON::MOUSE_BUTTON_5, 5}};

    bool InputManager::GetKey(KEYCODE key)
    {
        return s_KeysDown.contains(key) || s_KeysRepeat.contains(key);
    }

    bool InputManager::GetKeyDown(KEYCODE key)
    {
        return s_KeysDown.contains(key);
    }

    bool InputManager::GetKeyUp(KEYCODE key)
    {
        return s_KeysUp.contains(key);
    }

    bool InputManager::GetMouse(MOUSE_BUTTON mButton)
    {
        return s_MouseButtonStates[mButton] > 0;
    }

    bool InputManager::GetMouseDown(MOUSE_BUTTON mButton)
    {
        return s_MouseButtonStates[mButton] == 1;
    }

    bool InputManager::GetMouseUp(MOUSE_BUTTON mButton)
    {
        return s_MouseButtonStates[mButton] == 0;
    }

    void InputManager::SetMouseVisibility(bool visibilityState)
    {
        ImGui::SetMouseCursor(visibilityState ? ImGuiMouseCursor_Arrow : ImGuiMouseCursor_None);
    }

    glm::vec2 InputManager::GetMousePos()
    {
        return s_CurrentMousePos;
    }

    void InputManager::SetDeltaMousePos(float x, float y)
    {
        s_DeltaMousePos = {x, y};
    }

    glm::vec2 InputManager::GetDeltaMousePos()
    {
        return s_DeltaMousePos;
    }

    void InputManager::OnUpdate()
    {
        MOUSE_BUTTON mArr[] MOUSE_BUTTON_INITIALIZER;
        s_LastMousePos = s_CurrentMousePos;
        Uint32 mouseButtonMask = SDL_GetMouseState(&s_CurrentMousePos.x, &s_CurrentMousePos.y);

        for (const MOUSE_BUTTON &mCode : mArr)
        {
            if (mouseButtonMask & SDL_BUTTON(s_MouseCodeMap[mCode]))
            {
                // mousePressSet.insert(mCode);
                if (s_MouseButtonStates[mCode] < 2)
                {
                    s_MouseButtonStates[mCode]++;
                }
            }
            else
            {
                s_MouseButtonStates[mCode] = 0;
            }
        }

        SDL_GetRelativeMouseState(&s_DeltaMousePos.x, &s_DeltaMousePos.y);
    }

    void InputManager::ProcessKeyDown(SDL_Scancode key)
    {
        KEYCODE keycode = s_KeyCodeMap[key];

        if (s_KeysDown.contains(keycode))
        {
            s_KeysDown.erase(keycode);
            s_KeysRepeat.emplace(keycode);
        }
        else
        {
            s_KeysDown.emplace(keycode);
        }
    }

    void InputManager::ProcessKeyUp(SDL_Scancode key)
    {
        KEYCODE keycode = s_KeyCodeMap[key];

        if (s_KeysDown.contains(keycode))
        {
            s_KeysDown.erase(keycode);
        }

        if (s_KeysRepeat.contains(keycode))
        {
            s_KeysRepeat.erase(keycode);
        }

        s_KeysUp.emplace(keycode);
    }
}