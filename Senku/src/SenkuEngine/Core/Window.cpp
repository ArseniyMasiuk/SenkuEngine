#include "PrecompiledHeader.h"
#include "Window.h"
#include "Base.h"
#include "Platform\Windows\WindowsWindow.h"

namespace Senku
{
	Scope<Window> Senku::Window::Create(const WindowProps & props)
	{
#ifdef PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		ASSERT(false);
		return nullptr;
#endif
	}
}
