#include "Window/WindowManagerOpenGL.h"

namespace Dwarf {

    WindowManagerOpenGL::WindowManagerOpenGL(InputManagerOpenGL* inputManager) : inputManager(inputManager) { }

    void WindowManagerOpenGL::Init(){
        shouldWindowClose = false;
    }

    void WindowManagerOpenGL::Quit(){
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    GraphicsApi WindowManagerOpenGL::GetActiveApi(){
        return GraphicsApi::OpenGL;
    }

    void WindowManagerOpenGL::InitWindow(glm::ivec2 windowSize, glm::ivec2 minSize, const char* windowTitle){
        // SDL Setup
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        
        Uint32 WindowFlags = 0;
        WindowFlags |= SDL_WINDOW_OPENGL;
        WindowFlags |= SDL_WINDOW_HIDDEN;
        WindowFlags |= SDL_WINDOW_MOUSE_CAPTURE;
        WindowFlags |= SDL_WINDOW_RESIZABLE;

        window = SDL_CreateWindow(windowTitle, 0, 0, windowSize.x, windowSize.y, WindowFlags);
        if (window == NULL) {
            std::cout << "Failed to create window" << std::endl;
            SDL_Quit();
        }
        
        SDL_DisplayMode mode;
        int dmCode = SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(window), &mode);
        
        SDL_SetWindowMinimumSize(window, minSize.x, minSize.y);
        //SDL_SetWindowSize(window, windowSize.x, windowSize.y);
        SDL_SetWindowPosition(window, mode.w/2 - (windowSize.x / 2), mode.h / 2 - (windowSize.y / 2));
        context = SDL_GL_CreateContext(window);
        
        // OpenGL Setup
        gladLoadGL();
        glViewport(0, 0, windowSize.x, windowSize.y);
        SDL_GL_SetSwapInterval(0);
        glEnable(GL_DEPTH_TEST);
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SDL_GL_SwapWindow(window);

        // IMGUI Setup
        ImGui::CreateContext();
        ImGuiIO* io = &ImGui::GetIO();
        io->ConfigWindowsMoveFromTitleBarOnly = true;
        io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        //ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplSDL2_InitForOpenGL(window, context);
        ImGui_ImplOpenGL3_Init("#version 130");

        ImGui::StyleColorsDark();
        io->ConfigWindowsResizeFromEdges = true;
        io->Fonts->AddFontDefault();
        IWindowManager::fonts["normalTextFont"] = io->Fonts->AddFontFromFileTTF(INTER_REGULAR_PATH, 15);
        IWindowManager::fonts["largeTextFont"] = io->Fonts->AddFontFromFileTTF(INTER_REGULAR_PATH, 20);
        IWindowManager::fonts["smallHeaderFont"] = io->Fonts->AddFontFromFileTTF(INTER_BOLD_PATH, 20);
        IWindowManager::fonts["largeHeaderFont"] = io->Fonts->AddFontFromFileTTF(INTER_BOLD_PATH, 26);
        
        // Rendering info
        IWindowManager::rendererName = (char*)glad_glGetString(GL_RENDERER);
        IWindowManager::vendorName = (char*)glad_glGetString(GL_VENDOR);
        IWindowManager::apiVersion = std::string("OpenGL ") + (char*)glad_glGetString(GL_VERSION);
    }

    void WindowManagerOpenGL::SetWindowTitle(std::string windowName){
        SDL_SetWindowTitle(window, windowName.c_str());
    }

    void WindowManagerOpenGL::StartFrame(){
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
            {
                shouldWindowClose = true;
            }

            switch(event.type){
                case SDL_QUIT:
                    shouldWindowClose = true;
                    break;
                case SDL_KEYDOWN:
                    inputManager->ProcessKeyDown(event.key.keysym.scancode);
                    //std::cout << "Key down: " << event.key.keysym.sym << std::endl;
                    break;
                case SDL_KEYUP:
                    inputManager->ProcessKeyUp(event.key.keysym.scancode);
                    //std::cout << "Key up: " << event.key.keysym.sym << std::endl;
                    break;
            }
        }
        //glfwPollEvents();
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glfwGetWindowSize(window, &width, &height);
        SDL_GetWindowSize(window, &this->windowSize.x, &this->windowSize.y);
        //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
        glEnable(GL_DEPTH_TEST);

        ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplGlfw_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::PushFont(fonts["normalTextFont"]);
    }

    void WindowManagerOpenGL::EndFrame(){
        ImGui::PopFont();
        
        ImGuiIO* io = &ImGui::GetIO();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
        
        //glfwSwapBuffers(window);
        SDL_GL_SwapWindow(window);
    }

    glm::ivec2 WindowManagerOpenGL::GetWindowSize(){
        return this->windowSize;
    }

    bool WindowManagerOpenGL::ShouldWindowCloseSignal(){
        //return glfwWindowShouldClose(window);
        return shouldWindowClose;
    }

    void WindowManagerOpenGL::InitImGui(){
        
    }

    SDL_Window* WindowManagerOpenGL::GetOpenGLWindow(){
        return window;
    }

    IRenderTexture* WindowManagerOpenGL::AddRenderTexture(){
        renderTextures.push_back(new RenderTextureOpenGL());
        return renderTextures.at(renderTextures.size()-1);
    }

    void WindowManagerOpenGL::RemoveRenderTexture(int ID){
        for(int i = 0; i < renderTextures.size(); i++){
            if(renderTextures[i]->GetID() == ID){
                delete renderTextures[i];
                renderTextures.erase(renderTextures.begin()+i);
            }
        }
    }

    void WindowManagerOpenGL::LockCursorAtPos(glm::vec2 pos){
        SDL_WarpMouseInWindow(this->window, pos.x, pos.y);
    }

    void WindowManagerOpenGL::RelativeMouseMode(bool enabled){
        SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
    }

    void WindowManagerOpenGL::ShowWindow(){
        SDL_ShowWindow(window);
    }

    void WindowManagerOpenGL::HideWindow(){
        SDL_HideWindow(window);
    }
}