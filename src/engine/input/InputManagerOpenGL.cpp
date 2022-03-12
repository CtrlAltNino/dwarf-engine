#include"InputManagerOpenGL.h"
#include<GLFW/glfw3.h>

InputManagerOpenGL::InputManagerOpenGL(WindowManagerOpenGL* wm) : windowManager(wm){ }

bool InputManagerOpenGL::GetKey(KEYCODE key){
    return glfwGetKey(windowManager->GetOpenGLWindow(), keyCodeMap[key]) == GLFW_PRESS;
}

bool InputManagerOpenGL::GetKeyDown(KEYCODE key){
    return (keyPressSet.count(key) == 0) && glfwGetKey(windowManager->GetOpenGLWindow(), keyCodeMap[key]) == GLFW_PRESS;
}

bool InputManagerOpenGL::GetKeyUp(KEYCODE key){
    return (keyPressSet.count(key) != 0) && glfwGetKey(windowManager->GetOpenGLWindow(), keyCodeMap[key]) == GLFW_RELEASE;
}

bool InputManagerOpenGL::GetMouse(MOUSE_BUTTON mButton){
    return glfwGetMouseButton(windowManager->GetOpenGLWindow(), mouseCodeMap[mButton]) == GLFW_PRESS;
}

bool InputManagerOpenGL::GetMouseDown(MOUSE_BUTTON mButton){
    return (mousePressSet.count(mButton) == 0) && glfwGetMouseButton(windowManager->GetOpenGLWindow(), mouseCodeMap[mButton]) == GLFW_PRESS;
}

bool InputManagerOpenGL::GetMouseUp(MOUSE_BUTTON mButton){
    return (mousePressSet.count(mButton) != 0) && glfwGetMouseButton(windowManager->GetOpenGLWindow(), mouseCodeMap[mButton]) == GLFW_RELEASE;
}

void InputManagerOpenGL::SetMouseVisibility(bool visibilityState){
    glfwSetInputMode(windowManager->GetOpenGLWindow(), GLFW_CURSOR, visibilityState ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

glm::vec2 InputManagerOpenGL::GetMousePos(){
    double mouseX, mouseY;
    glfwGetCursorPos(windowManager->GetOpenGLWindow(), &mouseX, &mouseY);
    return glm::vec2(mouseX, mouseY);
}

void InputManagerOpenGL::UpdatePressStates(){
    // Update shit
    KEYCODE kArr[] KEYCODE_INITIALIZER;
    MOUSE_BUTTON mArr[] MOUSE_BUTTON_INITIALIZER;
    keyPressSet.clear();

    for(const KEYCODE &kCode : kArr){
        if(glfwGetKey(windowManager->GetOpenGLWindow(), keyCodeMap[kCode]) == GLFW_PRESS){
            keyPressSet.insert(kCode);
        }
    }

    mousePressSet.clear();

    for(const MOUSE_BUTTON &mCode : mArr){
        if(glfwGetMouseButton(windowManager->GetOpenGLWindow(), mouseCodeMap[mCode]) == GLFW_PRESS){
            mousePressSet.insert(mCode);
        }
    }
}