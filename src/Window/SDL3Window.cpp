#include "pch.h"

#include "Platform/OpenGL/OpenGLUtilities.h"
#include "Window/SDL3Window.h"
#include <SDL3/SDL_video.h>

namespace Dwarf
{
  SDL3Window::SDL3Window(
    const WindowProps&                       props,
    std::shared_ptr<IGraphicsContextFactory> contextFactory,
    std::shared_ptr<IImGuiLayerFactory>      imguiLayerFactory,
    std::shared_ptr<IInputManager>           inputManager,
    std::shared_ptr<IDwarfLogger>            logger,
    std::shared_ptr<IEditorStats>            editorStats)
    : mContextFactory(std::move(contextFactory))
    , mImguiLayerFactory(std::move(imguiLayerFactory))
    , mInputManager(std::move(inputManager))
    , mLogger(std::move(logger))
    , mEditorStats(std::move(editorStats))
  {
    mLogger->LogDebug(Log("Creating Windows Window", "SDL3Window"));
    mLogger->LogInfo(Log("Initializing Windows Window", "SDL3Window"));

    // SDL Setup
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
      mLogger->LogError(Log("Failed to initialize SDL", "SDL3Window"));
      SDL_Quit();
      return;
    }

    mData.Title = props.Title;
    mData.Height = props.Height;
    mData.Width = props.Width;
    mData.ShouldClose = false;

    Uint32 windowFlags = 0;
    windowFlags |= SDL_WINDOW_HIDDEN;
    windowFlags |= SDL_WINDOW_MOUSE_CAPTURE;

    if (props.Resizable)
    {
      windowFlags |= SDL_WINDOW_RESIZABLE;
    }

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

    mWindow = SDL_CreateWindow(
      mData.Title.c_str(), mData.Width, mData.Height, windowFlags);

    if (mWindow == nullptr)
    {
      mLogger->LogError(Log("Failed to create window", "SDL3Window"));
      SDL_Quit();
      return;
    }

    mLogger->LogInfo(Log("Window created", "SDL3Window"));

    const auto* mode =
      SDL_GetDesktopDisplayMode(SDL_GetDisplayForWindow(mWindow));

    SDL_SetWindowMinimumSize(mWindow, props.Width, props.Height);
    SDL_SetWindowPosition(mWindow,
                          (mode->w / 2) - (props.Width / 2),
                          (mode->h / 2) - (props.Height / 2));

    mLogger->LogDebug(Log("Creating Graphics Context...", "SDL3Window"));
    mContext = std::move(mContextFactory->Create(mWindow));

    if (mContext == nullptr)
    {
      mLogger->LogError(Log("Failed to create Graphics Context", "SDL3Window"));
      SDL_DestroyWindow(mWindow);
      SDL_Quit();
    }

    mLogger->LogDebug(Log("Graphics Context created", "SDL3Window"));

    mLogger->LogDebug(Log("Initializing Graphics Context...", "SDL3Window"));
    mContext->Init();
    mLogger->LogDebug(Log("Graphics Context initialized", "SDL3Window"));
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

    mEditorStats->SetDeviceInfo(deviceInfo);

    mLogger->LogDebug(Log("Creating ImGui Layer...", "SDL3Window"));
    mImGuiLayer = std::move(mImguiLayerFactory->Create());

    if (mImGuiLayer == nullptr)
    {
      mLogger->LogError(Log("Failed to create ImGui Layer", "SDL3Window"));
      SDL_DestroyWindow(mWindow);
      SDL_Quit();
    }

    mLogger->LogDebug(Log("ImGui Layer created", "SDL3Window"));

    mLogger->LogDebug(Log("Attaching ImGui Layer...", "SDL3Window"));
    mImGuiLayer->OnAttach(mWindow);

    mLogger->LogDebug(Log("ImGui Layer attached", "SDL3Window"));
    mLogger->LogInfo(Log("Windows Window initialized", "SDL3Window"));
    mLogger->LogDebug(Log("Windows Window created", "SDL3Window"));
  }

  SDL3Window::~SDL3Window()
  {
    mLogger->LogDebug(Log("Destroying Windows Window", "SDL3Window"));

    mLogger->LogDebug(Log("Detaching ImGui Layer", "SDL3Window"));
    mImGuiLayer->OnDetach();
    mLogger->LogDebug(Log("ImGui Layer detached", "SDL3Window"));

    mLogger->LogDebug(Log("Destroying SDL Window", "SDL3Window"));
    SDL_DestroyWindow(mWindow);
    mLogger->LogDebug(Log("SDL Window destroyed", "SDL3Window"));

    mLogger->LogDebug(Log("Quitting SDL", "SDL3Window"));
    SDL_Quit();
    mLogger->LogDebug(Log("SDL Quit", "SDL3Window"));

    mLogger->LogDebug(Log("Windows Window destroyed", "SDL3Window"));
  }

  void
  SDL3Window::ShowWindow()
  {
    mLogger->LogInfo(Log("Showing window", "SDL3Window"));
    if (mData.ShowMaximized)
    {
      MaximizeWindow();
      SDL_ShowWindow(mWindow);
    }
    else
    {
      SDL_ShowWindow(mWindow);
    }
  }

  void
  SDL3Window::HideWindow()
  {
    mLogger->LogInfo(Log("Hiding window", "SDL3Window"));
    SDL_HideWindow(mWindow);
  }

  void
  SDL3Window::NewFrame()
  {
    SDL_Event event;

    mInputManager->SetDeltaMousePos(0, 0);
    mInputManager->SetDeltaMouseScroll(0, 0);

    if (mData.IsRelativeMode)
    {
      SDL_WarpMouseInWindow(
        mWindow, mData.SavedMousePos.x, mData.SavedMousePos.y);
    }

    while (SDL_PollEvent(&event))
    {
      mImGuiLayer->HandleSDLEvent(&event);

      switch (event.type)
      {
        case SDL_EVENT_QUIT: mData.ShouldClose = true; break;
        case SDL_EVENT_KEY_DOWN:
          mInputManager->ProcessKeyDown(event.key.scancode);
          break;
        case SDL_EVENT_KEY_UP:
          mInputManager->ProcessKeyUp(event.key.scancode);
          break;
        case SDL_EVENT_MOUSE_WHEEL: mInputManager->ProcessScroll(event); break;
        default: break;
      }
    }
    mImGuiLayer->Begin();
  }

  void
  SDL3Window::EndFrame()
  {
    mImGuiLayer->End();
    mContext->SwapBuffers();
  }

  void
  SDL3Window::SetVSync(bool enabled)
  {
    mLogger->LogInfo(
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

    mData.VSync = enabled;
  }

  auto
  SDL3Window::IsVSync() -> bool
  {
    return mData.VSync;
  }

  auto
  SDL3Window::ShouldClose() -> bool
  {
    return mData.ShouldClose;
  }

  void
  SDL3Window::SetWindowTitle(std::string_view windowTitle)
  {
    mLogger->LogInfo(
      Log("Setting window title to " + std::string(windowTitle), "SDL3Window"));
    SDL_SetWindowTitle(mWindow, windowTitle.data());
  }

  void
  SDL3Window::MaximizeWindow()
  {
    mLogger->LogInfo(Log("Maximizing window", "SDL3Window"));
    SDL_MaximizeWindow(mWindow);
  }

  auto
  SDL3Window::IsWindowMaximized() -> bool
  {
    return (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MAXIMIZED) != 0U;
  }

  void
  SDL3Window::SetShowWindowMaximized(bool maximized)
  {
    mData.ShowMaximized = maximized;
  }

  void
  SDL3Window::SetMouseVisibility(bool visibilityState)
  {
    if (!visibilityState != mData.IsRelativeMode)
    {
      if (visibilityState)
      {
        SDL_WarpMouseInWindow(
          mWindow, mData.SavedMousePos.x, mData.SavedMousePos.y);
        SDL_SetWindowRelativeMouseMode(mWindow, !visibilityState);
      }
      else
      {
        SDL_GetMouseState(&mData.SavedMousePos.x, &mData.SavedMousePos.y);
        SDL_SetWindowRelativeMouseMode(mWindow, !visibilityState);
      }
      mData.IsRelativeMode = !visibilityState;
    }
  }
}