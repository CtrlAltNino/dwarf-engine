#include "dpch.h"

#include "Window/Window.h"

#ifdef _WIN32
#include "Platform/Windows/WindowsWindow.h"
#elif __linux__
#include "Platform/Linux/LinuxWindow.h"
#elif __APPLE__
#include "Platform/OSX/OSXWindow.h" // - NOT SUPPORTED YET
#endif

namespace Dwarf
{
  std::shared_ptr<Window>
  Window::Create(const WindowProps& props)
  {
    std::shared_ptr<Window> window = nullptr;
#ifdef _WIN32
    window = std::make_shared<WindowsWindow>(props.Api);
#elif __linux__
    window = std::make_shared<LinuxWindow>(props.Api);
#elif __APPLE__
    window = std::make_shared<OSXWindow>(props.Api); // - NOT SUPPORTED YET
                                                     // return nullptr;
#endif
    window->Init(props);
    return window;
  }
}