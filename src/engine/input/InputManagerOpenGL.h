#pragma once

#include "../../utilities/dpch.h"

#include"IInputManager.h"
#include<imgui_impl_opengl3.h>
#include<imgui_impl_sdl.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_opengl.h>
//#include"../windowing/WindowManagerOpenGL.h"

class InputManagerOpenGL : public IInputManager {
    private:
        /**
         * Pointer to the corresponding window manager
        */
        //WindowManagerOpenGL* windowManager;
        /**
         * State of the keyboard as received by SDL2
        */
        //const Uint8* keyboardState;
        /**
         * Saved position of the current mouse position
        */
        glm::ivec2 currentMousePos;
        glm::ivec2 lastMousePos;
        
        /**
         * State of the mouse buttons
        */
        std::map<MOUSE_BUTTON, int> mouseButtonStates;

        /*std::map<KEYCODE, int> keyCodeMap = {{KEYCODE::W, GLFW_KEY_W},
                                                {KEYCODE::A, GLFW_KEY_A},
                                                {KEYCODE::S, GLFW_KEY_S},
                                                {KEYCODE::D, GLFW_KEY_D},
                                                {KEYCODE::E, GLFW_KEY_E},
                                                {KEYCODE::Q, GLFW_KEY_Q},
                                                {KEYCODE::R, GLFW_KEY_R},
                                                {KEYCODE::LEFT_SHIFT, GLFW_KEY_LEFT_SHIFT},
                                                {KEYCODE::LEFT_CONTROL, GLFW_KEY_LEFT_CONTROL}};*/
        
        /**
         * Mapping the engine specific key codes to the SDL2 codes
        */
        std::map<SDL_Scancode, KEYCODE> keyCodeMap = {{SDL_SCANCODE_W, KEYCODE::W},
                                                {SDL_SCANCODE_A, KEYCODE::A},
                                                {SDL_SCANCODE_S, KEYCODE::S},
                                                {SDL_SCANCODE_D, KEYCODE::D},
                                                {SDL_SCANCODE_E, KEYCODE::E},
                                                {SDL_SCANCODE_Q, KEYCODE::Q},
                                                {SDL_SCANCODE_R, KEYCODE::R},
                                                {SDL_SCANCODE_LSHIFT, KEYCODE::LEFT_SHIFT},
                                                {SDL_SCANCODE_LCTRL, KEYCODE::LEFT_CONTROL}};
        
        /*std::map<KEYCODE, int> keyCodeMap = {{KEYCODE::W, SDL_SCANCODE_W},
                                                {KEYCODE::A, SDL_SCANCODE_A},
                                                {KEYCODE::S, SDL_SCANCODE_S},
                                                {KEYCODE::D, SDL_SCANCODE_D},
                                                {KEYCODE::E, SDL_SCANCODE_E},
                                                {KEYCODE::Q, SDL_SCANCODE_Q},
                                                {KEYCODE::R, SDL_SCANCODE_R},
                                                {KEYCODE::LEFT_SHIFT, SDL_SCANCODE_LSHIFT},
                                                {KEYCODE::LEFT_CONTROL, SDL_SCANCODE_LCTRL}};*/
        std::set<KEYCODE> keysDown;
        std::set<KEYCODE> keysRepeat;
        std::set<KEYCODE> keysUp;
        /*std::map<MOUSE_BUTTON, int> mouseCodeMap = {{MOUSE_BUTTON::LEFT, GLFW_MOUSE_BUTTON_LEFT},
                                                {MOUSE_BUTTON::RIGHT, GLFW_MOUSE_BUTTON_RIGHT},
                                                {MOUSE_BUTTON::MIDDLE, GLFW_MOUSE_BUTTON_MIDDLE}};*/
        
        /**
         * Mapping the engine specific key codes to the SDL2 codes
        */
        std::map<MOUSE_BUTTON, int> mouseCodeMap = {{MOUSE_BUTTON::LEFT, 1},
                                                {MOUSE_BUTTON::RIGHT, 3},
                                                {MOUSE_BUTTON::MIDDLE, 2}};
    public:
        //InputManagerOpenGL(WindowManagerOpenGL* wm);
        InputManagerOpenGL();
        /**
         * Returns true while the specified key is being pressed
        */
        virtual bool GetKey(KEYCODE key);
        /**
         * Returns true during the frame the specified key starts being pressed
        */
        virtual bool GetKeyDown(KEYCODE key);
        /**
         * Return true during the frame the specified key is being released
        */
        virtual bool GetKeyUp(KEYCODE key);
        
        /**
         * Returns true while the specified mouse button is being pressed
        */
        virtual bool GetMouse(MOUSE_BUTTON mButton);
        /**
         * Returns true during the frame the specified mouse button starts being pressed
        */
        virtual bool GetMouseDown(MOUSE_BUTTON mButton);
        /**
         * Return true during the frame the specified mouse button is being released
        */
        virtual bool GetMouseUp(MOUSE_BUTTON mButton);
        
        /**
         * Toggles the visibility of the cursor
        */
        virtual void SetMouseVisibility(bool visibilityState);

        /**
         * Returns the current position of the cursor in a vector
        */
        virtual glm::vec2 GetMousePos();

        virtual void SetDeltaMousePos(float x, float y);

        virtual glm::vec2 GetDeltaMousePos();

        /**
         * Updates the states of the input states.
         * Call this every frame
        */
        //virtual void UpdatePressStates();

        virtual void StartFrame();

        virtual void ProcessKeyDown(SDL_Scancode key);

        virtual void ProcessKeyUp(SDL_Scancode key);
};