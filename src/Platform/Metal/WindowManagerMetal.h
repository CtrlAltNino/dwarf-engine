/*#pragma once

// system
#include <iostream>

// Metal dependencies
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_metal.h>

// IMGUI
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include <imgui.h>
//#include<imgui_impl_metal.h>

// other
#include <stb_image.h>

#include "Core/Base.h"
#include "Window/IWindowManager.h"
#include "Input/InputManager.h"

namespace Dwarf {

    class WindowManagerMetal : public IWindowManager {
        private:
            /// @brief Reference to the SDL2 window.
            SDL_Window* window;

            SDL_Renderer* renderer;

            /// @brief Flag indicating if the window should be closed.
            bool shouldWindowClose;

            Ref<InputManagerOpenGL> inputManager;

        public:
            WindowManagerMetal(Ref<InputManager> inputManager);

            virtual void Quit();

            /// @brief Returns the used API.
            /// @return The used API represented as an enum.
            virtual GraphicsApi GetActiveApi();

            /// @brief Creates the window of the Dwarf Engine editor.
            virtual void InitWindow(glm::ivec2 windowSize, glm::ivec2 minSize, const char* windowTitle);

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

            virtual void RelativeMouseMode(bool enabled);

            /// @brief Executes all initializing IMGUI tasks.
            virtual void InitImGui();

            /// @brief Adds a new render texture.
            /// @return Pointer to the created render texture.
            virtual Ref<IRenderTexture> AddRenderTexture();

            /// @brief Removes the render texture with the given ID.
            /// @param ID ID of the render texture to remove.
            virtual void RemoveRenderTexture(int ID);

            /// @brief Returns the SDL2 Opengl window structure.
            /// @return SDL2 OpenGL window structure.
            SDL_Window* GetOpenGLWindow();

            virtual void ShowWindow();

            virtual void HideWindow();
    };
}*/