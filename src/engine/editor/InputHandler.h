#include<GLFW/glfw3.h>
#include"../windowing/WindowManager.h"
#include<glm/glm.hpp>
#include<glm/vec2.hpp>

enum class KEYCODE {KEYCODE_W, KEYCODE_A, KEYCODE_S, KEYCODE_D, KEYCODE_E, KEYCODE_Q, KEYCODE_LEFT_SHIFT};
enum class MOUSE_BUTTON {LEFT_MOUSE, MIDDLE_MOUSE, RIGHT_MOUSE};

class InputHandler{
    private:
        static bool GetKeyDown_GLFW(KEYCODE key){
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

            return glfwGetKey(WindowManager::GetWindow_OpenGL(), glfwKey) == GLFW_PRESS;
        }

        static bool GetMouseDown_GLFW(MOUSE_BUTTON mButton){
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

            return glfwGetMouseButton(WindowManager::GetWindow_OpenGL(), glfwMouse) == GLFW_PRESS;
        }
    public:
        static bool GetKeyDown(KEYCODE key){
            bool keyDown = false;
            
            switch(WindowManager::GetActiveGraphicsApi()){
                case GraphicsApi::OpenGL:
                    keyDown = GetKeyDown_GLFW(key);
                    break;
                case GraphicsApi::D3D11:
                    break;
                case GraphicsApi::D3D12:
                    break;
                case GraphicsApi::Vulkan:
                    break;
            }

            return keyDown;
        }

        static bool GetMouseDown(MOUSE_BUTTON mButton){
            bool mouseDown = false;
            
            switch(WindowManager::GetActiveGraphicsApi()){
                case GraphicsApi::OpenGL:
                    mouseDown = GetMouseDown_GLFW(mButton);
                    break;
                case GraphicsApi::D3D11:
                    break;
                case GraphicsApi::D3D12:
                    break;
                case GraphicsApi::Vulkan:
                    break;
            }

            return mouseDown;
        }

        static void SetMouseVisibility(bool visibilityState){
            switch(WindowManager::GetActiveGraphicsApi()){
                case GraphicsApi::OpenGL:
                    glfwSetInputMode(WindowManager::GetWindow_OpenGL(), GLFW_CURSOR, visibilityState ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
                    break;
                case GraphicsApi::D3D11:
                    break;
                case GraphicsApi::D3D12:
                    break;
                case GraphicsApi::Vulkan:
                    break;
            }
        }

        static glm::vec2 GetMousePos(){
            double mouseX, mouseY;
            
            switch(WindowManager::GetActiveGraphicsApi()){
                case GraphicsApi::OpenGL:
                    glfwGetCursorPos(WindowManager::GetWindow_OpenGL(), &mouseX, &mouseY);
                    break;
                case GraphicsApi::D3D11:
                    break;
                case GraphicsApi::D3D12:
                    break;
                case GraphicsApi::Vulkan:
                    break;
            }

            return glm::vec2(mouseX, mouseY);
        }
};