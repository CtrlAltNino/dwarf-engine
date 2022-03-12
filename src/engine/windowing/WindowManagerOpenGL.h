#pragma once

#include<glad/glad.h>
#include<imgui_internal.h>
#include<imgui_impl_opengl3.h>
#include<imgui_impl_glfw.h>
#include<stb_image.h>
#include<iostream>
#include<GLFW/glfw3.h>

#include"IWindowManager.h"
#include"../rendering/RenderTextureOpenGL.h"

class WindowManagerOpenGL : public IWindowManager{
    private:
        //static inline glm::vec4 clearColor = glm::vec4(0.1,0.1,0.1,1);
        GLFWwindow* window;
        GLuint fbo;
        GLuint textureColorBuffer;
        //std::vector<RenderTextureOpenGL> renderTextures;
    public:
        virtual void Init();
        virtual GraphicsApi GetActiveApi();
        virtual void CreateEditorWindow();
        virtual void SetWindowName(std::string windowName);
        virtual void StartFrame();
        virtual void EndFrame();
        virtual int GetWidth();
        virtual int GetHeight();
        virtual bool ShouldWindowCloseSignal();
        virtual void InitImGui();
        virtual IRenderTexture* AddRenderTexture();
        virtual void RemoveRenderTexture(int ID);

        GLFWwindow* GetOpenGLWindow();
};