#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>
#include<glm/glm.hpp>
#include<glm/vec4.hpp>
#include<string>

#define INITIAL_WINDOW_WIDTH (1280)
#define INITIAL_WINDOW_HEIGHT (720)

class WindowManager{
    private:
        static inline glm::vec4 clearColor = glm::vec4(0.1,0.1,0.1,1);
        static inline GLFWwindow* window_opengl;
        static inline GraphicsApi activeApi;
        static inline int width;
        static inline int height;
    public:
        static void InitWindowManager(GraphicsApi graphicsApi){
            activeApi = graphicsApi;
        }

        static GraphicsApi GetActiveGraphicsApi(){
            return activeApi;
        }
        
        static void CreateOpenGLWindow(std::string projectName){
            glfwInit();
            // get resolution of monitor
            GLFWmonitor* _monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(_monitor);
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
            glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

            window_opengl = glfwCreateWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, ("Dwarf Engine Editor - " + projectName + " - Scene name (OpenGL)").c_str(), NULL, NULL);
            
            if (window_opengl == NULL) {
                std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
            }

            glfwSetWindowSizeLimits(window_opengl, 1280, 720, GLFW_DONT_CARE, GLFW_DONT_CARE);

            GLFWimage icon[1];
            int numColChannel;
            icon[0].pixels = stbi_load("data/engine/img/icons/icon.png", &icon[0].width, &icon[0].height, &numColChannel, 0); //rgba channels
            glfwSetWindowIcon(window_opengl, 1, icon);
            stbi_image_free(icon[0].pixels);
            glfwMakeContextCurrent(window_opengl);
            gladLoadGL();
            //glfwSetWindowFocusCallback(window, window_focus_callback);
            glViewport(0, 0, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
            
            std::cout << "Window created with OpenGL version: " << glGetString(GL_VERSION) << std::endl;

            if (glfwRawMouseMotionSupported())
                glfwSetInputMode(window_opengl, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
            
            glfwSwapInterval(0);
            glEnable(GL_DEPTH_TEST);
            glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glfwSwapBuffers(window_opengl);
        }

        static void CreateEditorWindow(std::string projectName){
            switch(activeApi){
                case GraphicsApi::OpenGL:
                    CreateOpenGLWindow(projectName);
                    break;
                case GraphicsApi::D3D11:
                    break;
                case GraphicsApi::D3D12:
                    break;
                case GraphicsApi::Vulkan:
                    break;
            }
        }

        static void StartFrame(){
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glfwGetWindowSize(window_opengl, &width, &height);
        }

        static void EndFrame(){
            glfwSwapBuffers(window_opengl);
        }

        static GLFWwindow* GetWindow_OpenGL(){
            return window_opengl;
        }

        static int GetWidth(){
            return width;
        }

        static int GetHeight(){
            return height;
        }
};