#pragma once

#include "../../utilities/dpch.h"


#include<glm/glm.hpp>
#include<glm/vec2.hpp>

#include"../windowing/IWindowManager.h"

#define KEYCODE_INITIALIZER {W, A, S, D, E, Q, LEFT_SHIFT, LEFT_CONTROL}
#define MOUSE_BUTTON_INITIALIZER {LEFT, RIGHT, MIDDLE}
enum KEYCODE KEYCODE_INITIALIZER;
enum MOUSE_BUTTON MOUSE_BUTTON_INITIALIZER;

class IInputManager{
    protected:
        IWindowManager* windowManager;
        std::set<KEYCODE> keyPressSet;
        std::set<MOUSE_BUTTON> mousePressSet;
    public:
        virtual bool GetKey(KEYCODE key)=0;
        virtual bool GetKeyDown(KEYCODE key)=0;
        virtual bool GetKeyUp(KEYCODE key)=0;
        
        virtual bool GetMouse(MOUSE_BUTTON mButton)=0;
        virtual bool GetMouseDown(MOUSE_BUTTON mButton)=0;
        virtual bool GetMouseUp(MOUSE_BUTTON mButton)=0;
        
        virtual void SetMouseVisibility(bool visibilityState)=0;
        virtual glm::vec2 GetMousePos()=0;
        virtual void UpdatePressStates()=0;
};