#include"WindowManagerOpenGL.h"

void WindowManagerOpenGL::Init(){
    shouldWindowClose = false;
    CreateEditorWindow();
    InitImGui();
}

GraphicsApi WindowManagerOpenGL::GetActiveApi(){
    return GraphicsApi::OpenGL;
}

void WindowManagerOpenGL::CreateEditorWindow(){
    //glfwInit();
    // get resolution of monitor
    /*GLFWmonitor* _monitor = glfwGetPrimaryMonitor();
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
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);*/
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    
    Uint32 WindowFlags = 0;
	WindowFlags |= SDL_WINDOW_OPENGL;
	WindowFlags |= SDL_WINDOW_HIDDEN;
	WindowFlags |= SDL_WINDOW_MOUSE_CAPTURE;
	WindowFlags |= SDL_WINDOW_RESIZABLE;
	WindowFlags |= SDL_WINDOW_MAXIMIZED;

    //window = glfwCreateWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "Dwarf Engine Editor", NULL, NULL);
    window = SDL_CreateWindow("Dwarf Engine Project Launcher", 0, 0, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, WindowFlags);
    
	context = SDL_GL_CreateContext(window);
    
    if (window == NULL) {
        std::cout << "Failed to create window" << std::endl;
        SDL_Quit();
    }

    SDL_SetWindowMinimumSize(window, 1280, 720);
    //glfwSetWindowSizeLimits(window, 1280, 720, GLFW_DONT_CARE, GLFW_DONT_CARE);

    /*GLFWimage icon[1];
    int numColChannel;
    icon[0].pixels = stbi_load("data/engine/img/icons/icon.png", &icon[0].width, &icon[0].height, &numColChannel, 0); //rgba channels
    glfwSetWindowIcon(window, 1, icon);
    stbi_image_free(icon[0].pixels);
    glfwMakeContextCurrent(window);*/
    gladLoadGL();
    glViewport(0, 0, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);

    /*if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);*/
    
    //glfwSwapInterval(0);
    SDL_GL_SetSwapInterval(0);
    glEnable(GL_DEPTH_TEST);
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glfwSwapBuffers(window);
    SDL_GL_SwapWindow(window);

    
    IWindowManager::rendererName = (char*)glad_glGetString(GL_RENDERER);
    IWindowManager::vendorName = (char*)glad_glGetString(GL_VENDOR);
    IWindowManager::apiVersion = std::string("OpenGL ") + (char*)glad_glGetString(GL_VERSION);
}

void WindowManagerOpenGL::SetWindowTitle(std::string windowName){
    //glfwSetWindowTitle(window, windowName.c_str());
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
    std::cout << "Window Manager start frame END" << std::endl;
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