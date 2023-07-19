#include "dpch.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Input/InputManager.h"

namespace Dwarf {

    WindowsWindow::WindowsWindow(const WindowProps& props) : m_Api(props.Api) {
        Init(props);
    }

    WindowsWindow::~WindowsWindow() {
        m_ImguiLayer->OnDetach();
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
    }

    GraphicsApi WindowsWindow::GetApi(){
        return m_Api;
    }

    void WindowsWindow::Init(const WindowProps& props){
        // SDL Setup
        SDL_Init(SDL_INIT_VIDEO);

        m_Data.Title = props.Title;
        m_Data.Height = props.Height;
        m_Data.Width = props.Width;
        m_Data.ShouldClose = false;
        
        Uint32 WindowFlags = 0;
        WindowFlags |= SDL_WINDOW_HIDDEN;
        WindowFlags |= SDL_WINDOW_MOUSE_CAPTURE;
        WindowFlags |= SDL_WINDOW_RESIZABLE;
        
        switch(props.Api){
            case GraphicsApi::D3D12: break;
            case GraphicsApi::OpenGL:
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
                    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
                    WindowFlags |= SDL_WINDOW_OPENGL;
                break;
            case GraphicsApi::Vulkan:
                    WindowFlags |= SDL_WINDOW_VULKAN;
                break;
        }

        m_Window = SDL_CreateWindow(m_Data.Title.c_str(), 0, 0, (int)m_Data.Width, (int)m_Data.Height, WindowFlags);
        
        if (m_Window == NULL) {
            std::cout << "[WINDOW CREATION] Error: Failed to create window" << std::endl;
            SDL_Quit();
        }
        
        SDL_DisplayMode mode;
        int dmCode = SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(m_Window), &mode);
        
        SDL_SetWindowMinimumSize(m_Window, props.Width, props.Height);
        //SDL_SetWindowSize(window, windowSize.x, windowSize.y);
        SDL_SetWindowPosition(m_Window, mode.w/2 - (props.Width / 2), mode.h / 2 - (props.Height / 2));

        m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

        //SetVSync(true);

        m_ImguiLayer = ImGuiLayer::Create(m_Api);
        m_ImguiLayer->OnAttach(m_Window);

        //SDL_ShowWindow(m_Window);

        // OpenGL Setup
        //gladLoadGL();
        /*glViewport(0, 0, windowSize.x, windowSize.y);
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
        IWindowManager::apiVersion = std::string("OpenGL ") + (char*)glad_glGetString(GL_VERSION);*/
    }

    /*void WindowManagerOpenGL::SetWindowTitle(std::string windowName){
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

    Ref<IRenderTexture> WindowManagerOpenGL::AddRenderTexture(){
        renderTextures.push_back(CreateRef<RenderTextureOpenGL>(RenderTextureOpenGL()));
        return renderTextures.at(renderTextures.size()-1);
    }

    void WindowManagerOpenGL::RemoveRenderTexture(int ID){
        for(int i = 0; i < renderTextures.size(); i++){
            if(renderTextures[i]->GetID() == ID){
                //delete renderTextures[i];
                renderTextures.erase(renderTextures.begin()+i);
            }
        }
    }

    void WindowManagerOpenGL::LockCursorAtPos(glm::vec2 pos){
        SDL_WarpMouseInWindow(this->window, pos.x, pos.y);
    }

    void WindowManagerOpenGL::RelativeMouseMode(bool enabled){
        SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
    }*/

    void WindowsWindow::ShowWindow(){
        SDL_ShowWindow(m_Window);
    }

    void WindowsWindow::HideWindow(){
        SDL_HideWindow(m_Window);
    }

    void WindowsWindow::Shutdown()
	{
        //ImGui_ImplSDL2_Shutdown();
        //ImGui::DestroyContext();
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
	}

    void WindowsWindow::NewFrame(){
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            m_ImguiLayer->HandleSDLEvent(&event);

            switch(event.type){
                case SDL_QUIT:
                    m_Data.ShouldClose = true;
                    break;
                case SDL_KEYDOWN:
                    InputManager::ProcessKeyDown(event.key.keysym.scancode);
                    break;
                case SDL_KEYUP:
                    InputManager::ProcessKeyUp(event.key.keysym.scancode);
                    break;
            }
        }
        m_ImguiLayer->Begin();
    }

    void WindowsWindow::EndFrame(){
        m_ImguiLayer->End();
        m_Context->SwapBuffers();
    }

    void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			SDL_GL_SetSwapInterval(1);
		else
			SDL_GL_SetSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync()
	{
		return m_Data.VSync;
	}

    bool WindowsWindow::ShouldClose(){
        return m_Data.ShouldClose;
    }

    void WindowsWindow::SetWindowTitle(std::string windowName){
        SDL_SetWindowTitle(m_Window, windowName.c_str());
    }

    void WindowsWindow::MaximizeWindow(){
        SDL_MaximizeWindow(m_Window);
    }
}