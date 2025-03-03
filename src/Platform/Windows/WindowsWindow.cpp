#include "Platform/OpenGL/OpenGLUtilities.h"
#include "pch.h"
#include <SDL_video.h>
#include "Platform/Windows/WindowsWindow.h"

namespace Dwarf
{
  WindowsWindow::WindowsWindow(const WindowProps&            props,
                               IGraphicsContextFactory&      contextFactory,
                               IImGuiLayerFactory&           imguiLayerFactory,
                               IInputManager&                inputManager,
                               std::shared_ptr<IDwarfLogger> logger,
                               std::shared_ptr<IEditorStats> editorStats)
    : m_ContextFactory(contextFactory)
    , m_ImguiLayerFactory(imguiLayerFactory)
    , m_InputManager(inputManager)
    , m_Logger(logger)
    , m_EditorStats(editorStats)
  {
    m_Logger->LogDebug(Log("Creating Windows Window", "WindowsWindow"));
    Init(props);
    m_Logger->LogDebug(Log("Windows Window created", "WindowsWindow"));
  }

  WindowsWindow::~WindowsWindow()
  {
    m_Logger->LogDebug(Log("Destroying Windows Window", "WindowsWindow"));

    m_Logger->LogDebug(Log("Detaching ImGui Layer", "WindowsWindow"));
    m_ImGuiLayer->OnDetach();
    m_Logger->LogDebug(Log("ImGui Layer detached", "WindowsWindow"));

    m_Logger->LogDebug(Log("Destroying SDL Window", "WindowsWindow"));
    SDL_DestroyWindow(m_Window);
    m_Logger->LogDebug(Log("SDL Window destroyed", "WindowsWindow"));

    m_Logger->LogDebug(Log("Quitting SDL", "WindowsWindow"));
    SDL_Quit();
    m_Logger->LogDebug(Log("SDL Quit", "WindowsWindow"));

    m_Logger->LogDebug(Log("Windows Window destroyed", "WindowsWindow"));
  }

  void
  WindowsWindow::Init(const WindowProps& props)
  {
    m_Logger->LogInfo(Log("Initializing Windows Window", "WindowsWindow"));
    // SDL Setup
    int result = SDL_Init(SDL_INIT_VIDEO);

    if (result != 0)
    {
      m_Logger->LogError(
        Log("Failed to initialize SDL", "WindowsWindow", fmt::color::red));
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
        Log("Failed to create window", "WindowsWindow", fmt::color::red));
      SDL_Quit();
      return;
    }

    m_Logger->LogInfo(Log("Window created", "WindowsWindow"));

    SDL_DisplayMode mode;
    SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(m_Window), &mode);

    SDL_SetWindowMinimumSize(m_Window, props.Width, props.Height);
    SDL_SetWindowPosition(m_Window,
                          mode.w / 2 - (props.Width / 2),
                          mode.h / 2 - (props.Height / 2));

    m_Logger->LogDebug(Log("Creating Graphics Context...", "WindowsWindow"));
    m_Context = std::move(m_ContextFactory.Create(m_Window));

    if (m_Context == nullptr)
    {
      m_Logger->LogError(
        Log("Failed to create Graphics Context", "WindowsWindow"));
      SDL_DestroyWindow(m_Window);
      SDL_Quit();
    }

    m_Logger->LogDebug(Log("Graphics Context created", "WindowsWindow"));

    m_Logger->LogDebug(
      Log("Initializing Graphics Context...", "WindowsWindow"));
    m_Context->Init();
    m_Logger->LogDebug(Log("Graphics Context initialized", "WindowsWindow"));
    SDL_GL_SetSwapInterval(0);

    std::string deviceInfo = "";

    switch (props.Api)
    {
      using enum GraphicsApi;
      case D3D12: break;
      case OpenGL: deviceInfo = OpenGLUtilities::GetDeviceInfo(); break;
      case Vulkan: break;
      case Metal: break;
    }

    m_EditorStats->SetDeviceInfo(deviceInfo);

    m_Logger->LogDebug(Log("Creating ImGui Layer...", "WindowsWindow"));
    m_ImGuiLayer = std::move(m_ImguiLayerFactory.Create());

    if (m_ImGuiLayer == nullptr)
    {
      m_Logger->LogError(Log("Failed to create ImGui Layer", "WindowsWindow"));
      SDL_DestroyWindow(m_Window);
      SDL_Quit();
    }

    m_Logger->LogDebug(Log("ImGui Layer created", "WindowsWindow"));

    m_Logger->LogDebug(Log("Attaching ImGui Layer...", "WindowsWindow"));
    m_ImGuiLayer->OnAttach(m_Window);

    m_Logger->LogDebug(Log("ImGui Layer attached", "WindowsWindow"));
    m_Logger->LogInfo(Log("Windows Window initialized", "WindowsWindow"));
  }

  void
  WindowsWindow::ShowWindow()
  {
    m_Logger->LogInfo(Log("Showing window", "WindowsWindow"));
    SDL_ShowWindow(m_Window);
  }

  void
  WindowsWindow::HideWindow()
  {
    m_Logger->LogInfo(Log("Hiding window", "WindowsWindow"));
    SDL_HideWindow(m_Window);
  }

  void
  WindowsWindow::NewFrame()
  {
    SDL_Event event;

    m_InputManager.SetDeltaMousePos(0, 0);
    m_InputManager.SetDeltaMouseScroll(0, 0);

    while (SDL_PollEvent(&event))
    {
      m_ImGuiLayer->HandleSDLEvent(&event);

      switch (event.type)
      {
        case SDL_QUIT: m_Data.ShouldClose = true; break;
        case SDL_KEYDOWN:
          m_InputManager.ProcessKeyDown(event.key.keysym.scancode);
          break;
        case SDL_KEYUP:
          m_InputManager.ProcessKeyUp(event.key.keysym.scancode);
          break;
        case SDL_MOUSEWHEEL: m_InputManager.ProcessScroll(event); break;
        default: break;
      }
    }
    m_ImGuiLayer->Begin();
  }

  void
  WindowsWindow::EndFrame()
  {
    m_ImGuiLayer->End();
    m_Context->SwapBuffers();
  }

  void
  WindowsWindow::SetVSync(bool enabled)
  {
    m_Logger->LogInfo(
      Log("Setting VSync to " + std::to_string(enabled), "WindowsWindow"));
    if (enabled)
      SDL_GL_SetSwapInterval(1);
    else
      SDL_GL_SetSwapInterval(0);

    m_Data.VSync = enabled;
  }

  bool
  WindowsWindow::IsVSync()
  {
    return m_Data.VSync;
  }

  bool
  WindowsWindow::ShouldClose()
  {
    return m_Data.ShouldClose;
  }

  void
  WindowsWindow::SetWindowTitle(std::string_view windowName)
  {
    m_Logger->LogInfo(Log("Setting window title to " + std::string(windowName),
                          "WindowsWindow"));
    SDL_SetWindowTitle(m_Window, windowName.data());
  }

  void
  WindowsWindow::MaximizeWindow()
  {
    m_Logger->LogInfo(Log("Maximizing window", "WindowsWindow"));
    SDL_MaximizeWindow(m_Window);
  }

  bool
  WindowsWindow::IsWindowMaximized()
  {
    return SDL_GetWindowFlags(m_Window) & SDL_WINDOW_MAXIMIZED;
  }
}