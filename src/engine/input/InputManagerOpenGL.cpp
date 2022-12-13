#include"InputManagerOpenGL.h"

InputManagerOpenGL::InputManagerOpenGL(){
    this->mouseButtonStates = std::map<MOUSE_BUTTON, int>();
}

bool InputManagerOpenGL::GetKey(KEYCODE key){
    //return glfwGetKey(windowManager->GetOpenGLWindow(), keyCodeMap[key]) == GLFW_PRESS;
    //return keyboardState[keyCodeMap[key]];
    return keysDown.contains(key) || keysRepeat.contains(key);
}

bool InputManagerOpenGL::GetKeyDown(KEYCODE key){
    //return (keyPressSet.count(key) == 0) && glfwGetKey(windowManager->GetOpenGLWindow(), keyCodeMap[key]) == GLFW_PRESS;
    //return keyboardState[keyCodeMap[key]];
    return keysDown.contains(key);
}

bool InputManagerOpenGL::GetKeyUp(KEYCODE key){
    //return (keyPressSet.count(key) != 0) && glfwGetKey(windowManager->GetOpenGLWindow(), keyCodeMap[key]) == GLFW_RELEASE;
    //return !keyboardState[keyCodeMap[key]];
    return keysUp.contains(key);
}

bool InputManagerOpenGL::GetMouse(MOUSE_BUTTON mButton){
    //return glfwGetMouseButton(windowManager->GetOpenGLWindow(), mouseCodeMap[mButton]) == GLFW_PRESS;
    return this->mouseButtonStates[mButton] > 0;
}

bool InputManagerOpenGL::GetMouseDown(MOUSE_BUTTON mButton){
    //return (mousePressSet.count(mButton) == 0) && glfwGetMouseButton(windowManager->GetOpenGLWindow(), mouseCodeMap[mButton]) == GLFW_PRESS;
    return this->mouseButtonStates[mButton] == 1;
}

bool InputManagerOpenGL::GetMouseUp(MOUSE_BUTTON mButton){
    //return (mousePressSet.count(mButton) != 0) && glfwGetMouseButton(windowManager->GetOpenGLWindow(), mouseCodeMap[mButton]) == GLFW_RELEASE;
    return this->mouseButtonStates[mButton] == 0;
}

void InputManagerOpenGL::SetMouseVisibility(bool visibilityState){
    SDL_ShowCursor(visibilityState);
    //glfwSetInputMode(windowManager->GetOpenGLWindow(), GLFW_CURSOR, visibilityState ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

glm::vec2 InputManagerOpenGL::GetMousePos(){
    //double mouseX, mouseY;
    //glfwGetCursorPos(windowManager->GetOpenGLWindow(), &mouseX, &mouseY);
    //return glm::vec2(mouseX, mouseY);
    return this->currentMousePos;
}

/*void InputManagerOpenGL::UpdatePressStates(){
    // Update shit
    SDL_PumpEvents();
    this->keyboardState = SDL_GetKeyboardState(NULL);
    KEYCODE kArr[] KEYCODE_INITIALIZER;
    MOUSE_BUTTON mArr[] MOUSE_BUTTON_INITIALIZER;
    //keyPressSet.clear();
    Uint32 mouseButtonMask = SDL_GetMouseState(&this->currentMousePos.x, &this->currentMousePos.y);

    for(const KEYCODE &kCode : kArr){
        //if(glfwGetKey(windowManager->GetOpenGLWindow(), keyCodeMap[kCode]) == GLFW_PRESS){
        if(keyboardState[keyCodeMap[kCode]] == 1){
            //keyPressSet.insert(kCode);
        }
    }

    //mousePressSet.clear();

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
}*/

void InputManagerOpenGL::StartFrame(){
    //keysDown
    MOUSE_BUTTON mArr[] MOUSE_BUTTON_INITIALIZER;
    //keyPressSet.clear();
    Uint32 mouseButtonMask = SDL_GetMouseState(&this->currentMousePos.x, &this->currentMousePos.y);

    //mousePressSet.clear();

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
}

void InputManagerOpenGL::ProcessKeyDown(SDL_Scancode key){
    std::cout << "Processing key down: " << key << std::endl;
    KEYCODE keycode = keyCodeMap[key];

    if(keysDown.contains(keycode)){
        keysDown.erase(keycode);
        keysRepeat.emplace(keycode);
    }else{
        keysDown.emplace(keycode);
    }
}

void InputManagerOpenGL::ProcessKeyUp(SDL_Scancode key){
    std::cout << "Processing key up: " << key << std::endl;
    KEYCODE keycode = keyCodeMap[key];

    if(keysDown.contains(keycode)){
        keysDown.erase(keycode);
    }
    
    if(keysRepeat.contains(keycode)){
        keysRepeat.erase(keycode);
    }

    keysUp.emplace(keycode);
}