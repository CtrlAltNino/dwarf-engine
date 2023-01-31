#include "WindowManagerMetal.h"

WindowManagerMetal::WindowManagerMetal(InputManagerOpenGL* inputManager) : inputManager(inputManager) { }

void WindowManagerMetal::Init(){
    shouldWindowClose = false;
}

void WindowManagerMetal::Quit(){
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
	SDL_DestroyWindow(window);
    SDL_Quit();
}

GraphicsApi WindowManagerMetal::GetActiveApi(){
    return GraphicsApi::Metal;
}

void WindowManagerMetal::CreateWindow(glm::ivec2 windowSize, glm::ivec2 minSize, const char* windowTitle){
    // SDL Setup
    Uint32 WindowFlags = 0;
	WindowFlags |= SDL_WINDOW_HIDDEN;
	WindowFlags |= SDL_WINDOW_MOUSE_CAPTURE;
	WindowFlags |= SDL_WINDOW_RESIZABLE;
	WindowFlags |= SDL_WINDOW_MAXIMIZED;
    
    window = SDL_CreateWindow(windowTitle, 0, 0, windowSize.x, windowSize.y, WindowFlags);
    if (window == NULL) {
        std::cout << "Failed to create window" << std::endl;
        SDL_Quit();
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    NS::Error *err;
    SDL_DisplayMode mode;
	int dmCode = SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(window), &mode);

    SDL_SetWindowSize(window, windowSize.x, windowSize.y);
	SDL_SetWindowPosition(window, mode.w/2 - (windowSize.x / 2), mode.h / 2 - (windowSize.y / 2));
    SDL_SetWindowMinimumSize(window, minSize.x, minSize.y);

    // IMGUI Setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);
    ImGuiIO* io = &ImGui::GetIO();
    io->ConfigWindowsMoveFromTitleBarOnly = true;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    IWindowManager::fonts["normalTextFont"] = io->Fonts->AddFontFromFileTTF(INTER_REGULAR_PATH, 15);
    IWindowManager::fonts["largeTextFont"] = io->Fonts->AddFontFromFileTTF(INTER_REGULAR_PATH, 20);
    IWindowManager::fonts["smallHeaderFont"] = io->Fonts->AddFontFromFileTTF(INTER_BOLD_PATH, 20);
    IWindowManager::fonts["largeHeaderFont"] = io->Fonts->AddFontFromFileTTF(INTER_BOLD_PATH, 26);

    // Rendering info
    MTL::Device* dev = MTL::CreateSystemDefaultDevice();
    IWindowManager::rendererName = dev->name()->cString(NS::UTF8StringEncoding);
}

void WindowManagerMetal::SetWindowTitle(std::string windowName){
    SDL_SetWindowTitle(window, windowName.c_str());
}

void WindowManagerMetal::StartFrame(){
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
    //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
    //glEnable(GL_DEPTH_TEST);

    //ImGui_ImplMetal_NewFrame();
    //ImGui_ImplOpenGL3_NewFrame();
    //ImGui_ImplGlfw_NewFrame();

    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    //ImGui::Render();
    //ImGui::PushFont(fonts["normalTextFont"]);
}

void WindowManagerMetal::EndFrame(){
    //ImGui::PopFont();
    
    ImGuiIO* io = &ImGui::GetIO();
    ImGui::Render();
    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData());
    //SDL_RenderClear(this->renderer);
    ImGui_ImplSDLRenderer_RenderDrawData(
        ImGui::GetDrawData()
    );
    SDL_RenderPresent(
        this->renderer
    );
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
    
    //glfwSwapBuffers(window);
    //SDL_GL_SwapWindow(window);
    //SDL_Metal_SwapWindow(window);
}

glm::ivec2 WindowManagerMetal::GetWindowSize(){
    return this->windowSize;
}

bool WindowManagerMetal::ShouldWindowCloseSignal(){
    //return glfwWindowShouldClose(window);
    return shouldWindowClose;
}

void WindowManagerMetal::InitImGui(){
    ImGui::CreateContext();
    ImGuiIO* io = &ImGui::GetIO();
    io->ConfigWindowsMoveFromTitleBarOnly = true;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    //ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplSDL2_InitForMetal(window);
    //ImGui_ImplSDL2_InitForOpenGL(window, context);
    //ImGui_ImplOpenGL3_Init("#version 130");
    //ImGui_ImplMetal_Init();

    ImGui::StyleColorsDark();
    io->ConfigWindowsResizeFromEdges = true;
    io->Fonts->AddFontDefault();
    IWindowManager::fonts["normalTextFont"] = io->Fonts->AddFontFromFileTTF(INTER_REGULAR_PATH, 15);
    IWindowManager::fonts["largeTextFont"] = io->Fonts->AddFontFromFileTTF(INTER_REGULAR_PATH, 20);
    IWindowManager::fonts["smallHeaderFont"] = io->Fonts->AddFontFromFileTTF(INTER_BOLD_PATH, 20);
    IWindowManager::fonts["largeHeaderFont"] = io->Fonts->AddFontFromFileTTF(INTER_BOLD_PATH, 26);
}

SDL_Window* WindowManagerMetal::GetOpenGLWindow(){
    return window;
}

IRenderTexture* WindowManagerMetal::AddRenderTexture(){
    //renderTextures.push_back(new RenderTextureOpenGL());
    return renderTextures.at(renderTextures.size()-1);
}

void WindowManagerMetal::RemoveRenderTexture(int ID){
    for(int i = 0; i < renderTextures.size(); i++){
        if(renderTextures[i]->GetID() == ID){
            delete renderTextures[i];
            renderTextures.erase(renderTextures.begin()+i);
        }
    }
}

void WindowManagerMetal::LockCursorAtPos(glm::vec2 pos){
    SDL_WarpMouseInWindow(this->window, pos.x, pos.y);
}

void WindowManagerMetal::RelativeMouseMode(bool enabled){
    SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
}

void WindowManagerMetal::ShowWindow(){
    SDL_ShowWindow(window);
}

void WindowManagerMetal::HideWindow(){
    SDL_HideWindow(window);
}