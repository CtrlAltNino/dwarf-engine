#include"WindowManagerOpenGL.h"


void WindowManagerOpenGL::Init(){
    CreateEditorWindow();
    InitImGui();
}

GraphicsApi WindowManagerOpenGL::GetActiveApi(){
    return GraphicsApi::OpenGL;
}


void WindowManagerOpenGL::CreateEditorWindow(){
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

    window = glfwCreateWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "Dwarf Engine Editor", NULL, NULL);
    
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwSetWindowSizeLimits(window, 1280, 720, GLFW_DONT_CARE, GLFW_DONT_CARE);

    GLFWimage icon[1];
    int numColChannel;
    icon[0].pixels = stbi_load("data/engine/img/icons/icon.png", &icon[0].width, &icon[0].height, &numColChannel, 0); //rgba channels
    glfwSetWindowIcon(window, 1, icon);
    stbi_image_free(icon[0].pixels);
    glfwMakeContextCurrent(window);
    gladLoadGL();
    //glfwSetWindowFocusCallback(window, window_focus_callback);
    glViewport(0, 0, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
    
    std::cout << "Window created with OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    
    glfwSwapInterval(0);
    glEnable(GL_DEPTH_TEST);
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window);

    //unsigned int framebuffer;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);  

    // generate texture
    unsigned int textureColorBuffer;
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

void WindowManagerOpenGL::SetWindowName(std::string windowName){
    glfwSetWindowTitle(window, windowName.c_str());
}

void WindowManagerOpenGL::StartFrame(){
    glfwPollEvents();
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwGetWindowSize(window, &width, &height);
    //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
    glEnable(GL_DEPTH_TEST);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void WindowManagerOpenGL::EndFrame(){
    ImGuiIO* io = &ImGui::GetIO();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
    
    glfwSwapBuffers(window);
}

int WindowManagerOpenGL::GetWidth(){
    return width;
}

int WindowManagerOpenGL::GetHeight(){
    return height;
}

bool WindowManagerOpenGL::ShouldWindowCloseSignal(){
    return glfwWindowShouldClose(window);
}

void WindowManagerOpenGL::InitImGui(){
    ImGui::CreateContext();
    ImGuiIO* io = &ImGui::GetIO();
    
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGui::StyleColorsDark();
    io->ConfigWindowsResizeFromEdges = true;
}

GLFWwindow* WindowManagerOpenGL::GetOpenGLWindow(){
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