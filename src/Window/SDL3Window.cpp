#include "Window/SDL3Window.h"
#include "Platform/OpenGL/OpenGLUtilities.h"
#include "pch.h"
#include <SDL3/SDL_video.h>
#include <utility>

namespace Dwarf
{
  SDL3Window::SDL3Window(
    const WindowProps&                       props,
    std::shared_ptr<IGraphicsContextFactory> contextFactory,
    std::shared_ptr<IImGuiLayerFactory>      imguiLayerFactory,
    std::shared_ptr<IInputManager>           inputManager,
    std::shared_ptr<IDwarfLogger>            logger,
    std::shared_ptr<IEditorStats>            editorStats)
    : m_ContextFactory(std::move(contextFactory))
    , m_ImguiLayerFactory(std::move(imguiLayerFactory))
    , m_InputManager(std::move(inputManager))
    , m_Logger(std::move(logger))
    , m_EditorStats(std::move(editorStats))
  {
    m_Logger->LogDebug(Log("Creating Windows Window", "SDL3Window"));
    m_Logger->LogInfo(Log("Initializing Windows Window", "SDL3Window"));

    // SDL Setup
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
      m_Logger->LogError(Log("Failed to initialize SDL", "SDL3Window"));
      SDL_Quit();
      return;
    }

    m_Data.Title = props.Title;
    m_Data.Height = props.Height;
    m_Data.Width = props.Width;
    m_Data.ShouldClose = false;

    Uint32 windowFlags = 0;
    windowFlags |= SDL_WINDOW_HIDDEN;
    windowFlags |= SDL_WINDOW_MOUSE_CAPTURE;
    windowFlags |= SDL_WINDOW_RESIZABLE;

    switch (props.Api)
    {
      using enum GraphicsApi;
      case None:
      case D3D12: break;
      case OpenGL:
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
        windowFlags |= SDL_WINDOW_OPENGL;
        break;
      case Vulkan: windowFlags |= SDL_WINDOW_VULKAN; break;
      case Metal:
        // NOT SUPPORTED
        break;
    }

    m_Window = SDL_CreateWindow(
      m_Data.Title.c_str(), (int)m_Data.Width, (int)m_Data.Height, windowFlags);

    if (m_Window == nullptr)
    {
      m_Logger->LogError(Log("Failed to create window", "SDL3Window"));
      SDL_Quit();
      return;
    }

    m_Logger->LogInfo(Log("Window created", "SDL3Window"));

    const auto* mode =
      SDL_GetDesktopDisplayMode(SDL_GetDisplayForWindow(m_Window));

    SDL_SetWindowMinimumSize(m_Window, props.Width, props.Height);
    SDL_SetWindowPosition(m_Window,
                          mode->w / 2 - (props.Width / 2),
                          mode->h / 2 - (props.Height / 2));

    m_Logger->LogDebug(Log("Creating Graphics Context...", "SDL3Window"));
    m_Context = std::move(m_ContextFactory->Create(m_Window));

    if (m_Context == nullptr)
    {
      m_Logger->LogError(
        Log("Failed to create Graphics Context", "SDL3Window"));
      SDL_DestroyWindow(m_Window);
      SDL_Quit();
    }

    m_Logger->LogDebug(Log("Graphics Context created", "SDL3Window"));

    m_Logger->LogDebug(Log("Initializing Graphics Context...", "SDL3Window"));
    m_Context->Init();
    m_Logger->LogDebug(Log("Graphics Context initialized", "SDL3Window"));
    SDL_GL_SetSwapInterval(0);

    std::string deviceInfo;

    switch (props.Api)
    {
      using enum GraphicsApi;
      case OpenGL: deviceInfo = OpenGLUtilities::GetDeviceInfo(); break;
      case None:
      case D3D12:
      case Vulkan:
      case Metal: break;
    }

    m_EditorStats->SetDeviceInfo(deviceInfo);

    m_Logger->LogDebug(Log("Creating ImGui Layer...", "SDL3Window"));
    m_ImGuiLayer = std::move(m_ImguiLayerFactory->Create());

    if (m_ImGuiLayer == nullptr)
    {
      m_Logger->LogError(Log("Failed to create ImGui Layer", "SDL3Window"));
      SDL_DestroyWindow(m_Window);
      SDL_Quit();
    }

    m_Logger->LogDebug(Log("ImGui Layer created", "SDL3Window"));

    m_Logger->LogDebug(Log("Attaching ImGui Layer...", "SDL3Window"));
    m_ImGuiLayer->OnAttach(m_Window);

    m_Logger->LogDebug(Log("ImGui Layer attached", "SDL3Window"));
    m_Logger->LogInfo(Log("Windows Window initialized", "SDL3Window"));
    m_Logger->LogDebug(Log("Windows Window created", "SDL3Window"));
  }

  SDL3Window::~SDL3Window()
  {
    m_Logger->LogDebug(Log("Destroying Windows Window", "SDL3Window"));

    m_Logger->LogDebug(Log("Detaching ImGui Layer", "SDL3Window"));
    m_ImGuiLayer->OnDetach();
    m_Logger->LogDebug(Log("ImGui Layer detached", "SDL3Window"));

    m_Logger->LogDebug(Log("Destroying SDL Window", "SDL3Window"));
    SDL_DestroyWindow(m_Window);
    m_Logger->LogDebug(Log("SDL Window destroyed", "SDL3Window"));

    m_Logger->LogDebug(Log("Quitting SDL", "SDL3Window"));
    SDL_Quit();
    m_Logger->LogDebug(Log("SDL Quit", "SDL3Window"));

    m_Logger->LogDebug(Log("Windows Window destroyed", "SDL3Window"));
  }

  void
  SDL3Window::showWindow()
  {
    m_Logger->LogInfo(Log("Showing window", "SDL3Window"));
    if (m_Data.ShowMaximized)
    {
      maximizeWindow();
      SDL_ShowWindow(m_Window);
    }
    else
    {
      SDL_ShowWindow(m_Window);
    }
  }

  void
  SDL3Window::hideWindow()
  {
    m_Logger->LogInfo(Log("Hiding window", "SDL3Window"));
    SDL_HideWindow(m_Window);
  }

  void
  SDL3Window::newFrame()
  {
    SDL_Event event;

    m_InputManager->SetDeltaMousePos(0, 0);
    m_InputManager->SetDeltaMouseScroll(0, 0);

    while (SDL_PollEvent(&event))
    {
      m_ImGuiLayer->HandleSDLEvent(&event);

      switch (event.type)
      {
        case SDL_EVENT_QUIT: m_Data.ShouldClose = true; break;
        case SDL_EVENT_KEY_DOWN:
          m_InputManager->ProcessKeyDown(event.key.scancode);
          break;
        case SDL_EVENT_KEY_UP:
          m_InputManager->ProcessKeyUp(event.key.scancode);
          break;
        case SDL_EVENT_MOUSE_WHEEL: m_InputManager->ProcessScroll(event); break;
        default: break;
      }
    }
    m_ImGuiLayer->Begin();
  }

  void
  SDL3Window::endFrame()
  {
    m_ImGuiLayer->End();
    m_Context->SwapBuffers();
  }

  void
  SDL3Window::setVSync(bool enabled)
  {
    m_Logger->LogInfo(
      Log("Setting VSync to " + std::to_string(static_cast<int>(enabled)),
          "SDL3Window"));
    if (enabled)
    {
      SDL_GL_SetSwapInterval(1);
    }
    else
    {
      SDL_GL_SetSwapInterval(0);
    }

    m_Data.VSync = enabled;
  }

  auto
  SDL3Window::isVSync() -> bool
  {
    return m_Data.VSync;
  }

  auto
  SDL3Window::shouldClose() -> bool
  {
    return m_Data.ShouldClose;
  }

  void
  SDL3Window::setWindowTitle(std::string_view windowTitle)
  {
    m_Logger->LogInfo(
      Log("Setting window title to " + std::string(windowTitle), "SDL3Window"));
    SDL_SetWindowTitle(m_Window, windowTitle.data());
  }

  void
  SDL3Window::maximizeWindow()
  {
    m_Logger->LogInfo(Log("Maximizing window", "SDL3Window"));
    SDL_MaximizeWindow(m_Window);
  }

  auto
  SDL3Window::isWindowMaximized() -> bool
  {
    return (SDL_GetWindowFlags(m_Window) & SDL_WINDOW_MAXIMIZED) != 0U;
  }

  void
  SDL3Window::setShowWindowMaximized(bool maximized)
  {
    m_Data.ShowMaximized = maximized;
  }
}