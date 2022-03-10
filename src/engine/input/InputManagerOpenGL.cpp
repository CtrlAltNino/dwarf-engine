#include"InputManagerOpenGL.h"
#include<GLFW/glfw3.h>

InputManagerOpenGL::InputManagerOpenGL(WindowManagerOpenGL* wm) : windowManager(wm){ }

bool InputManagerOpenGL::GetKeyDown(KEYCODE key){
    int glfwKey = -1;
    switch(key){
        case KEYCODE::KEYCODE_A:
            glfwKey = GLFW_KEY_A;
            break;
        case KEYCODE::KEYCODE_D:
            glfwKey = GLFW_KEY_D;
            break;
        case KEYCODE::KEYCODE_S:
            glfwKey = GLFW_KEY_S;
            break;
        case KEYCODE::KEYCODE_W:
            glfwKey = GLFW_KEY_W;
            break;
        case KEYCODE::KEYCODE_Q:
            glfwKey = GLFW_KEY_Q;
            break;
        case KEYCODE::KEYCODE_E:
            glfwKey = GLFW_KEY_E;
            break;
        case KEYCODE::KEYCODE_LEFT_SHIFT:
            glfwKey = GLFW_KEY_LEFT_SHIFT;
            break;
    }

    return glfwGetKey(windowManager->GetOpenGLWindow(), glfwKey) == GLFW_PRESS;
}

bool InputManagerOpenGL::GetMouseDown(MOUSE_BUTTON mButton){
    int glfwMouse = -1;
    switch(mButton){
        case MOUSE_BUTTON::LEFT_MOUSE:
            glfwMouse = GLFW_MOUSE_BUTTON_LEFT;
            break;
        case MOUSE_BUTTON::MIDDLE_MOUSE:
            glfwMouse = GLFW_MOUSE_BUTTON_MIDDLE;
            break;
        case MOUSE_BUTTON::RIGHT_MOUSE:
            glfwMouse = GLFW_MOUSE_BUTTON_RIGHT;
            break;
    }

    return glfwGetMouseButton(windowManager->GetOpenGLWindow(), glfwMouse) == GLFW_PRESS;
}

void InputManagerOpenGL::SetMouseVisibility(bool visibilityState){
    glfwSetInputMode(windowManager->GetOpenGLWindow(), GLFW_CURSOR, visibilityState ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

glm::vec2 InputManagerOpenGL::GetMousePos(){
    double mouseX, mouseY;
    glfwGetCursorPos(windowManager->GetOpenGLWindow(), &mouseX, &mouseY);
    return glm::vec2(mouseX, mouseY);
}