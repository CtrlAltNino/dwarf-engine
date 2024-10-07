#include "pch.h"
#include "Platform/Linux/LinuxWindow.h"

namespace Dwarf
{
  LinuxWindow::LinuxWindow(
    const WindowProps&                       props,
    std::shared_ptr<IGraphicsContextFactory> contextFactory,
    std::shared_ptr<IImGuiLayerFactory>      imguiLayerFactory,
    std::shared_ptr<IInputManager>           inputManager,
    std::shared_ptr<IDwarfLogger>            logger)
    : m_ContextFactory(contextFactory)
    , m_ImguiLayerFactory(imguiLayerFactory)
    , m_InputManager(inputManager)
    , m_Logger(logger)
  {
    m_Logger->LogDebug(Log("Creating Linux Window...", "LinuxWindow"));
    Init(props);
    m_Logger->LogDebug(Log("Linux Window created", "LinuxWindow"));
  }

  LinuxWindow::~LinuxWindow()
  {
    m_Logger->LogDebug(Log("Destroying Linux Window...", "LinuxWindow"));

    m_Logger->LogDebug(Log("Detaching ImGui Layer...", "LinuxWindow"));
    m_ImGuiLayer->OnDetach();
    m_Logger->LogDebug(Log("ImGui Layer detached", "LinuxWindow"));

    m_Logger->LogDebug(Log("Destroying SDL Window...", "LinuxWindow"));
    SDL_DestroyWindow(m_Window);
    m_Logger->LogDebug(Log("SDL Window destroyed", "LinuxWindow"));

    m_Logger->LogDebug(Log("Quitting SDL...", "LinuxWindow"));
    SDL_Quit();
    m_Logger->LogDebug(Log("SDL Quit", "LinuxWindow"));

    m_Logger->LogDebug(Log("Linux Window destroyed", "LinuxWindow"));
  }

  void
  LinuxWindow::Init(const WindowProps& props)
  {
    m_Logger->LogDebug(Log("Initializing Linux Window...", "LinuxWindow"));
    // SDL Setup
    int result = SDL_Init(SDL_INIT_VIDEO);

    if (result != 0)
    {
      m_Logger->LogError(
        Log("Failed to initialize SDL", "LinuxWindow", fmt::color::red));
      SDL_Quit();
      return;
    }

    m_Data.Title = props.Title;
    m_Data.Height = props.Height;
    m_Data.Width = props.Width;
    m_Data.ShouldClose = false;

    Uint32 WindowFlags = 0;
    WindowFlags |= SDL_WINDOW_HIDDEN;
    WindowFlags |= SDL_WINDOW_MOUSE_CAPTURE;
    WindowFlags |= SDL_WINDOW_RESIZABLE;

    switch (props.Api)
    {
      using enum GraphicsApi;
      case D3D12: break;
      case OpenGL:
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
        WindowFlags |= SDL_WINDOW_OPENGL;
        break;
      case Vulkan: WindowFlags |= SDL_WINDOW_VULKAN; break;
      case Metal:
        // NOT SUPPORTED
        break;
    }

    m_Window = SDL_CreateWindow(m_Data.Title.c_str(),
                                0,
                                0,
                                (int)m_Data.Width,
                                (int)m_Data.Height,
                                WindowFlags);

    if (m_Window == nullptr)
    {
      m_Logger->LogError(
        Log("Failed to create window", "LinuxWindow", fmt::color::red));
      SDL_Quit();
      return;
    }

    m_Logger->LogDebug(Log("Window created", "LinuxWindow"));

    SDL_DisplayMode mode;
    SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(m_Window), &mode);

    SDL_SetWindowMinimumSize(m_Window, props.Width, props.Height);
    SDL_SetWindowPosition(m_Window,
                          mode.w / 2 - (props.Width / 2),
                          mode.h / 2 - (props.Height / 2));

    m_Logger->LogDebug(Log("Creating Graphics Context...", "LinuxWindow"));
    m_Context = m_ContextFactory->Create(m_Window);

    if (m_Context == nullptr)
    {
      m_Logger->LogError(
        Log("Failed to create Graphics Context", "LinuxWindow"));
      SDL_DestroyWindow(m_Window);
      SDL_Quit();
    }

    m_Logger->LogDebug(Log("Graphics Context created", "LinuxWindow"));

    m_Logger->LogDebug(Log("Initializing Graphics Context...", "LinuxWindow"));
    m_Context->Init();
    m_Logger->LogDebug(Log("Graphics Context initialized", "LinuxWindow"));

    m_Logger->LogDebug(Log("Creating ImGui Layer...", "LinuxWindow"));
    m_ImGuiLayer = m_ImguiLayerFactory->Create();

    if (m_ImGuiLayer == nullptr)
    {
      m_Logger->LogError(Log("Failed to create ImGui Layer", "LinuxWindow"));
      SDL_DestroyWindow(m_Window);
      SDL_Quit();
    }

    m_Logger->LogDebug(Log("ImGui Layer created", "LinuxWindow"));

    m_Logger->LogDebug(Log("Attaching ImGui Layer...", "LinuxWindow"));
    m_ImGuiLayer->OnAttach(m_Window);

    m_Logger->LogDebug(Log("ImGui Layer attached", "LinuxWindow"));
    m_Logger->LogDebug(Log("Linux Window initialized", "LinuxWindow"));
  }

  void
  LinuxWindow::ShowWindow()
  {
    m_Logger->LogInfo(Log("Showing Window...", "LinuxWindow"));
    SDL_ShowWindow(m_Window);
  }

  void
  LinuxWindow::HideWindow()
  {
    m_Logger->LogInfo(Log("Hiding Window...", "LinuxWindow"));
    SDL_HideWindow(m_Window);
  }

  void
  LinuxWindow::NewFrame()
  {
    SDL_Event event;

    m_InputManager->SetDeltaMousePos(0, 0);
    m_InputManager->SetDeltaMouseScroll(0, 0);

    while (SDL_PollEvent(&event))
    {
      m_ImGuiLayer->HandleSDLEvent(&event);

      switch (event.type)
      {
        case SDL_QUIT: m_Data.ShouldClose = true; break;
        case SDL_KEYDOWN:
          m_InputManager->ProcessKeyDown(event.key.keysym.scancode);
          break;
        case SDL_KEYUP:
          m_InputManager->ProcessKeyUp(event.key.keysym.scancode);
          break;
        case SDL_MOUSEWHEEL: m_InputManager->ProcessScroll(event); break;
        default: break;
      }
    }
    m_ImGuiLayer->Begin();
  }

  void
  LinuxWindow::EndFrame()
  {
    m_ImGuiLayer->End();
    m_Context->SwapBuffers();
  }

  void
  LinuxWindow::SetVSync(bool enabled)
  {
    m_Logger->LogInfo(
      Log("Setting VSync to " + std::to_string(enabled), "LinuxWindow"));
    if (enabled)
      SDL_GL_SetSwapInterval(1);
    else
      SDL_GL_SetSwapInterval(0);

    m_Data.VSync = enabled;
  }

  bool
  LinuxWindow::IsVSync()
  {
    return m_Data.VSync;
  }

  bool
  LinuxWindow::ShouldClose()
  {
    return m_Data.ShouldClose;
  }

  void
  LinuxWindow::SetWindowTitle(std::string_view windowName)
  {
    m_Logger->LogInfo(Log(fmt::format("Setting Window Title to {}", windowName),
                          "LinuxWindow"));
    SDL_SetWindowTitle(m_Window, windowName.data());
  }

  void
  LinuxWindow::MaximizeWindow()
  {
    m_Logger->LogInfo(Log("Maximizing Window...", "LinuxWindow"));
    SDL_MaximizeWindow(m_Window);
  }
}