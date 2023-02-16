#include "Input/InputManagerOpenGL.h"

namespace Dwarf {

    InputManagerOpenGL::InputManagerOpenGL(){
        this->mouseButtonStates = std::map<MOUSE_BUTTON, int>();
    }

    bool InputManagerOpenGL::GetKey(KEYCODE key){
        return keysDown.contains(key) || keysRepeat.contains(key);
    }

    bool InputManagerOpenGL::GetKeyDown(KEYCODE key){
        return keysDown.contains(key);
    }

    bool InputManagerOpenGL::GetKeyUp(KEYCODE key){
        return keysUp.contains(key);
    }

    bool InputManagerOpenGL::GetMouse(MOUSE_BUTTON mButton){
        return this->mouseButtonStates[mButton] > 0;
    }

    bool InputManagerOpenGL::GetMouseDown(MOUSE_BUTTON mButton){
        return this->mouseButtonStates[mButton] == 1;
    }

    bool InputManagerOpenGL::GetMouseUp(MOUSE_BUTTON mButton){
        return this->mouseButtonStates[mButton] == 0;
    }

    void InputManagerOpenGL::SetMouseVisibility(bool visibilityState){
        ImGui::SetMouseCursor(visibilityState ? ImGuiMouseCursor_Arrow : ImGuiMouseCursor_None);
    }

    glm::vec2 InputManagerOpenGL::GetMousePos(){
        return this->currentMousePos;
    }

    void InputManagerOpenGL::SetDeltaMousePos(float x, float y){
        this->deltaMousePos.x = x;
        this->deltaMousePos.y = y;
    }

    glm::vec2 InputManagerOpenGL::GetDeltaMousePos(){
        return this->deltaMousePos;
    }

    void InputManagerOpenGL::StartFrame(){
        MOUSE_BUTTON mArr[] MOUSE_BUTTON_INITIALIZER;
        this->lastMousePos = this->currentMousePos;
        Uint32 mouseButtonMask = SDL_GetMouseState(&this->currentMousePos.x, &this->currentMousePos.y);

        for(const MOUSE_BUTTON &mCode : mArr){
            if(mouseButtonMask&SDL_BUTTON(mouseCodeMap[mCode])){
                //mousePressSet.insert(mCode);
                if(mouseButtonStates[mCode] < 2){
                    mouseButtonStates[mCode]++;
                }
            }else{
                mouseButtonStates[mCode] = 0;
            }
        }

        SDL_GetRelativeMouseState(&this->deltaMousePos.x, &this->deltaMousePos.y);
    }

    void InputManagerOpenGL::ProcessKeyDown(SDL_Scancode key){
        KEYCODE keycode = keyCodeMap[key];

        if(keysDown.contains(keycode)){
            keysDown.erase(keycode);
            keysRepeat.emplace(keycode);
        }else{
            keysDown.emplace(keycode);
        }
    }

    void InputManagerOpenGL::ProcessKeyUp(SDL_Scancode key){
        KEYCODE keycode = keyCodeMap[key];

        if(keysDown.contains(keycode)){
            keysDown.erase(keycode);
        }
        
        if(keysRepeat.contains(keycode)){
            keysRepeat.erase(keycode);
        }

        keysUp.emplace(keycode);
    }
}