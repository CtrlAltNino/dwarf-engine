#pragma once

#include"../windowing/IWindowManager.h"

#include<glm/glm.hpp>
#include<glm/vec2.hpp>

enum class KEYCODE {KEYCODE_W, KEYCODE_A, KEYCODE_S, KEYCODE_D, KEYCODE_E, KEYCODE_Q, KEYCODE_LEFT_SHIFT};
enum class MOUSE_BUTTON {LEFT_MOUSE, MIDDLE_MOUSE, RIGHT_MOUSE};

class IInputManager{
    private:
        IWindowManager* windowManager;
    public:
        virtual bool GetKeyDown(KEYCODE key)=0;
        virtual bool GetMouseDown(MOUSE_BUTTON mButton)=0;
        virtual void SetMouseVisibility(bool visibilityState)=0;
        virtual glm::vec2 GetMousePos()=0;
};