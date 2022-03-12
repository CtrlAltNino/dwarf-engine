#pragma once

#include"IInputManager.h"
#include"../windowing/WindowManagerOpenGL.h"
#include<map>

class InputManagerOpenGL : public IInputManager {
    private:
        WindowManagerOpenGL* windowManager;

        std::map<KEYCODE, int> keyCodeMap = {{KEYCODE::W, GLFW_KEY_W},
                                                {KEYCODE::A, GLFW_KEY_A},
                                                {KEYCODE::S, GLFW_KEY_S},
                                                {KEYCODE::D, GLFW_KEY_D},
                                                {KEYCODE::E, GLFW_KEY_E},
                                                {KEYCODE::Q, GLFW_KEY_Q},
                                                {KEYCODE::LEFT_SHIFT, GLFW_KEY_LEFT_SHIFT}};
        
        std::map<MOUSE_BUTTON, int> mouseCodeMap = {{MOUSE_BUTTON::LEFT, GLFW_MOUSE_BUTTON_LEFT},
                                                {MOUSE_BUTTON::RIGHT, GLFW_MOUSE_BUTTON_RIGHT},
                                                {MOUSE_BUTTON::MIDDLE, GLFW_MOUSE_BUTTON_MIDDLE}};
    public:
        InputManagerOpenGL(WindowManagerOpenGL* wm);
        virtual bool GetKey(KEYCODE key);
        virtual bool GetKeyDown(KEYCODE key);
        virtual bool GetKeyUp(KEYCODE key);
        
        virtual bool GetMouse(MOUSE_BUTTON mButton);
        virtual bool GetMouseDown(MOUSE_BUTTON mButton);
        virtual bool GetMouseUp(MOUSE_BUTTON mButton);
        
        virtual void SetMouseVisibility(bool visibilityState);
        virtual glm::vec2 GetMousePos();
        virtual void UpdatePressStates();
};