#pragma once

#include <imgui_impl_sdl.h>
#include <SDL2/SDL.h>
#include <glm/vec2.hpp>

#include "Core/Base.h"

namespace Dwarf
{

#define KEYCODE_INITIALIZER                           \
    {                                                 \
        W, A, S, D, E, Q, R, LEFT_SHIFT, LEFT_CONTROL \
    }
#define MOUSE_BUTTON_INITIALIZER                            \
    {                                                       \
        LEFT, RIGHT, MIDDLE, MOUSE_BUTTON_4, MOUSE_BUTTON_5 \
    }
    enum KEYCODE KEYCODE_INITIALIZER;
    enum MOUSE_BUTTON MOUSE_BUTTON_INITIALIZER;

    class InputManager
    {
    private:
        /**
         * Saved position of the current mouse position
         */
        static glm::ivec2 s_CurrentMousePos;
        static glm::ivec2 s_LastMousePos;
        static glm::ivec2 s_DeltaMousePos;

        /**
         * State of the mouse buttons
         */
        static std::map<MOUSE_BUTTON, int> s_MouseButtonStates;

        /**
         * Mapping the engine specific key codes to the SDL2 codes
         */
        static std::map<SDL_Scancode, KEYCODE> s_KeyCodeMap;
        static std::set<KEYCODE> s_KeysDown;
        static std::set<KEYCODE> s_KeysRepeat;
        static std::set<KEYCODE> s_KeysUp;

        /**
         * Mapping the engine specific key codes to the SDL2 codes
         */
        static std::map<MOUSE_BUTTON, int> s_MouseCodeMap;

    public:
        /**
         * Returns true while the specified key is being pressed
         */
        static bool GetKey(KEYCODE key);
        /**
         * Returns true during the frame the specified key starts being pressed
         */
        static bool GetKeyDown(KEYCODE key);
        /**
         * Return true during the frame the specified key is being released
         */
        static bool GetKeyUp(KEYCODE key);

        /**
         * Returns true while the specified mouse button is being pressed
         */
        static bool GetMouse(MOUSE_BUTTON mButton);
        /**
         * Returns true during the frame the specified mouse button starts being pressed
         */
        static bool GetMouseDown(MOUSE_BUTTON mButton);
        /**
         * Return true during the frame the specified mouse button is being released
         */
        static bool GetMouseUp(MOUSE_BUTTON mButton);

        /**
         * Toggles the visibility of the cursor
         */
        static void SetMouseVisibility(bool visibilityState);

        /**
         * Returns the current position of the cursor in a vector
         */
        static glm::vec2 GetMousePos();

        static void SetDeltaMousePos(float x, float y);

        static glm::vec2 GetDeltaMousePos();

        /**
         * Updates the states of the input states.
         * Call this every frame
         */

        static void OnUpdate();

        static void ProcessKeyDown(SDL_Scancode key);

        static void ProcessKeyUp(SDL_Scancode key);
    };
}