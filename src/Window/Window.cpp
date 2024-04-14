#include "dpch.h"

#include "Window/Window.h"

#ifdef _WIN32
#include "Platform/Windows/WindowsWindow.h"
#elif __linux__
#include "Platform/Linux/LinuxWindow.h"
#elif __APPLE__
#include "Platform/OSX/OSXWindow.h" // - NOT SUPPORTED YET
#endif

namespace Dwarf {
  Ref<Window> Window::Create(const WindowProps& props)
  {
    Ref<Window> window = nullptr;
#ifdef _WIN32
    window = CreateRef<WindowsWindow>(props.Api);
#elif __linux__
    window = CreateRef<LinuxWindow>(props.Api);
#elif __APPLE__
    window = CreateRef<OSXWindow>(props.Api); // - NOT SUPPORTED YET
                                              // return nullptr;
#endif
    window->Init(props);
    return window;
  }
}