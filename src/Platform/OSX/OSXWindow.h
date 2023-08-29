#pragma once

// system
#include <iostream>

// SDL
#include <SDL2/SDL.h>

#include "Window/Window.h"
#include "Core/Rendering/GraphicsContext.h"
#include "Core/UI/ImGuiLayer.h"

namespace Dwarf {

    class OSXWindow : public Window{
        public:
            OSXWindow(const WindowProps& props);
            virtual ~OSXWindow();

            void NewFrame() override;
            void EndFrame() override;

            unsigned int GetWidth() const override { return m_Data.Width; }
            unsigned int GetHeight() const override { return m_Data.Height; }

            virtual void ShowWindow() override;
            virtual void HideWindow() override;

            // Window attributes
            //void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
            void SetVSync(bool enabled) override;
            bool IsVSync() override;

            bool ShouldClose() override;

            void SetWindowTitle(std::string windowTitle) override;

            virtual void* GetNativeWindow() const { return m_Window; }

            virtual GraphicsApi GetApi() override;

            virtual void MaximizeWindow() override;
        private:
            virtual void Init(const WindowProps& props);
            virtual void Shutdown();
        private:
            SDL_Window* m_Window;
            Scope<GraphicsContext> m_Context;
            GraphicsApi m_Api;
            Ref<ImGuiLayer> m_ImguiLayer;

            struct WindowData
            {
                std::string Title;
                unsigned int Width, Height;
                bool VSync;
                bool ShouldClose;

                //EventCallbackFn EventCallback;
            };

            WindowData m_Data;
    };
}