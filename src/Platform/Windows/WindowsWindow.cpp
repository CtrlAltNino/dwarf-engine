#include "pch.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Input/InputManager.h"

namespace Dwarf
{
  WindowsWindow::WindowsWindow(const WindowProps& props)
    : m_Api(props.Api)
  {
    Init(props);
  }

  WindowsWindow::~WindowsWindow()
  {
    m_ImguiLayer->OnDetach();
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
  }

  GraphicsApi
  WindowsWindow::GetApi()
  {
    return m_Api;
  }

  void
  WindowsWindow::Init(const WindowProps& props)
  {
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
      std::cout << "[WINDOW CREATION] Error: Failed to create window"
                << std::endl;
      SDL_Quit();
    }

    SDL_DisplayMode mode;
    SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(m_Window), &mode);

    SDL_SetWindowMinimumSize(m_Window, props.Width, props.Height);
    SDL_SetWindowPosition(m_Window,
                          mode.w / 2 - (props.Width / 2),
                          mode.h / 2 - (props.Height / 2));

    m_Context = GraphicsContext::Create(m_Window);
    m_Context->Init();

    m_ImguiLayer = ImGuiLayer::Create(m_Api);
    m_ImguiLayer->OnAttach(m_Window);
  }

  void
  WindowsWindow::ShowWindow()
  {
    SDL_ShowWindow(m_Window);
  }

  void
  WindowsWindow::HideWindow()
  {
    SDL_HideWindow(m_Window);
  }

  void
  WindowsWindow::NewFrame()
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      m_ImguiLayer->HandleSDLEvent(&event);

      switch (event.type)
      {
        case SDL_QUIT: m_Data.ShouldClose = true; break;
        case SDL_KEYDOWN:
          InputManager::ProcessKeyDown(event.key.keysym.scancode);
          break;
        case SDL_KEYUP:
          InputManager::ProcessKeyUp(event.key.keysym.scancode);
          break;
        case SDL_MOUSEWHEEL: InputManager::ProcessScroll(event); break;
        default: break;
      }
    }
    m_ImguiLayer->Begin();
  }

  void
  WindowsWindow::EndFrame()
  {
    m_ImguiLayer->End();
    m_Context->SwapBuffers();
  }

  void
  WindowsWindow::SetVSync(bool enabled)
  {
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
    SDL_SetWindowTitle(m_Window, windowName.data());
  }

  void
  WindowsWindow::MaximizeWindow()
  {
    SDL_MaximizeWindow(m_Window);
  }
}