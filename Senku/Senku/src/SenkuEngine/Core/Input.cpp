#include "Input.h"
#include "Platform\Windows\WindowsInput.h"
namespace Senku
{
	Input* Input::s_Instance = new WindowsInput();
}