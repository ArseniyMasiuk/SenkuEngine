#include "PrecompiledHeader.h"
#include "Input.h"
#include "Platform\Windows\WindowsInput.h"
namespace Senku
{
	Ref<Input> Input::s_Instance = CreateRef<WindowsInput>();
}