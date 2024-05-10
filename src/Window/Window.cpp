#include "pch.h"
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
  std::unique_ptr<Window>
  Window::Create(const WindowProps& props)
  {
    std::unique_ptr<Window> window = nullptr;
#ifdef _WIN32
    window = std::make_unique<WindowsWindow>(props);
#elif __linux__
    window = std::make_unique<LinuxWindow>(props);
#elif __APPLE__
    window = std::make_unique<OSXWindow>(props); // - NOT SUPPORTED YET
                                                 // return nullptr;
#endif
    return window;
  }
}