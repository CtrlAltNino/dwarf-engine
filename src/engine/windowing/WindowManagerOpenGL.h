#pragma once

#include<glad/glad.h>
#include<imgui_impl_opengl3.h>
#include<imgui_impl_sdl.h>
#include<stb_image.h>
#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_opengl.h>

#include "../../utilities/dpch.h"

#include"IWindowManager.h"
#include"../rendering/RenderTextureOpenGL.h"
#include"../input/InputManagerOpenGL.h"

class WindowManagerOpenGL : public IWindowManager{
    private:
        /// @brief Reference to the SDL2 window.
        SDL_Window* window;

        /// @brief OpenGL handle of the window's frame buffer object.
        GLuint fbo;

        /// @brief OpenGL handle of the window's texture color buffer.
        GLuint textureColorBuffer;

        /// @brief Flag indicating if the window should be closed.
        bool shouldWindowClose;

        /// @brief Reference to the OpenGL context of SDL2.
        SDL_GLContext context;

        InputManagerOpenGL* inputManager;
        //std::vector<RenderTextureOpenGL> renderTextures;
    public:
        WindowManagerOpenGL(InputManagerOpenGL* inputManager);
        /// @brief Initializes the window manager.
        virtual void Init();

        /// @brief Returns the used API.
        /// @return The used API represented as an enum.
        virtual GraphicsApi GetActiveApi();

        /// @brief Creates the window of the Dwarf Engine editor.
        virtual void CreateEditorWindow();

        /// @brief Changes the title of the window.
        /// @param windowName The new title.
        virtual void SetWindowTitle(std::string windowName);

        /// @brief Executing all window related tasks needed to be done in the beginning of a frame.
        virtual void StartFrame();

        /// @brief Executing all window related tasks needed to be done at the end of a frame.
        virtual void EndFrame();

        /// @brief Returns the size of the window.
        /// @return Integer vector containing the size of the window.
        virtual glm::ivec2 GetWindowSize();

        /// @brief Returns if the window should be closed.
        /// @return True if the window should be closed, false if not.
        virtual bool ShouldWindowCloseSignal();
        
        virtual void LockCursorAtPos(glm::vec2 pos);

        virtual void RelativeMouseMode(boolean enabled);

        /// @brief Executes all initializing IMGUI tasks.
        virtual void InitImGui();

        /// @brief Adds a new render texture.
        /// @return Pointer to the created render texture.
        virtual IRenderTexture* AddRenderTexture();

        /// @brief Removes the render texture with the given ID.
        /// @param ID ID of the render texture to remove.
        virtual void RemoveRenderTexture(int ID);

        /// @brief Returns the SDL2 Opengl window structure.
        /// @return SDL2 OpenGL window structure.
        SDL_Window* GetOpenGLWindow();
};