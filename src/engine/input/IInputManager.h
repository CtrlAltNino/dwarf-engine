#pragma once

#include "../../utilities/dpch.h"

#include<glm/glm.hpp>
#include<glm/vec2.hpp>

#include"../windowing/IWindowManager.h"

#define KEYCODE_INITIALIZER {W, A, S, D, E, Q, R, LEFT_SHIFT, LEFT_CONTROL}
#define MOUSE_BUTTON_INITIALIZER {LEFT, RIGHT, MIDDLE}
enum KEYCODE KEYCODE_INITIALIZER;
enum MOUSE_BUTTON MOUSE_BUTTON_INITIALIZER;

/// @brief Interace for the input manager.
class IInputManager{
    protected:
        /// @brief Pointer to the window manager.
        IWindowManager* windowManager;

        glm::ivec2 deltaMousePos;
        //std::set<KEYCODE> keyPressSet;
        //std::set<MOUSE_BUTTON> mousePressSet;
    public:
        /**
         * Returns true while the specified key is being pressed
        */
        virtual bool GetKey(KEYCODE key)=0;
        /**
         * Returns true during the frame the specified key starts being pressed
        */
        virtual bool GetKeyDown(KEYCODE key)=0;
        /**
         * Return true during the frame the specified key is being released
        */
        virtual bool GetKeyUp(KEYCODE key)=0;

        /**
         * Returns true while the specified mouse button is being pressed
        */
        virtual bool GetMouse(MOUSE_BUTTON mButton)=0;
        /**
         * Returns true during the frame the specified mouse button starts being pressed
        */
        virtual bool GetMouseDown(MOUSE_BUTTON mButton)=0;
        /**
         * Return true during the frame the specified mouse button is being released
        */
        virtual bool GetMouseUp(MOUSE_BUTTON mButton)=0;
        
        /**
         * Toggles the visibility of the cursor
        */
        virtual void SetMouseVisibility(bool visibilityState)=0;

        /**
         * Returns the current position of the cursor in a vector
        */
        virtual glm::vec2 GetMousePos()=0;

        virtual void SetDeltaMousePos(float x, float y)=0;

        virtual glm::vec2 GetDeltaMousePos()=0;

        /**
         * Updates the states of the input states.
         * Call this every frame
        */
        //virtual void UpdatePressStates()=0;

        virtual void StartFrame()=0;
};