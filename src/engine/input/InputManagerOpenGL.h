#pragma once

#include"IInputManager.h"
#include"../windowing/WindowManagerOpenGL.h"

class InputManagerOpenGL : public IInputManager {
    private:
        WindowManagerOpenGL* windowManager;
    public:
        InputManagerOpenGL(WindowManagerOpenGL* wm);
        virtual bool GetKeyDown(KEYCODE key);
        virtual bool GetMouseDown(MOUSE_BUTTON mButton);
        virtual void SetMouseVisibility(bool visibilityState);
        virtual glm::vec2 GetMousePos();
};