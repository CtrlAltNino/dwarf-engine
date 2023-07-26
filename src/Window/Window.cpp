#include "dpch.h"

#include "Window/Window.h"

#ifdef _WIN32
#include "Platform/Windows/WindowsWindow.h"
#elif __linux__
//#include "Platform/Windows/LinuxWindow.h" - NOT SUPPORTED YET
#elif __APPLE__
//#include "Platform/OSX/OSXWindow.h" - NOT SUPPORTED YET
#endif

namespace Dwarf {
    Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef _WIN32
		return CreateScope<WindowsWindow>(props);
#elif __linux__
		//return CreateScope<LinuxWindow>(props); - NOT SUPPORTED YET
#elif __APPLE__
		//return CreateScope<OSXWindow>(props); - NOT SUPPORTED YET
#endif
	}
}