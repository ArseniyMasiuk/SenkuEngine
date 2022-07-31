#include "PrecompiledHeader.h"
#include "SystemUtils.h"

#include "SenkuEngine\Core\Application.h"
#ifdef PLATFORM_WINDOWS

#include "GLFW\glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW\glfw3native.h"

#include <commdlg.h>
#endif // PLATFORM_WINDOWS



namespace Senku
{

#ifdef PLATFORM_WINDOWS

	std::string FileDialog::OpenFile(const char* filter)
	{
		OPENFILENAME ofn;       // common dialog box structure
		char szFile[260];       // buffer for file name
		HWND hwnd;              // owner window
		HANDLE hf;              // file handle

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get()->GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		// Display the Open dialog box. 

		if (GetOpenFileName(&ofn) == TRUE)
			return std::string(ofn.lpstrFile);

		return std::string();
	}

	std::string FileDialog::SaveFile(const char* filter)
	{
		OPENFILENAME ofn;       // common dialog box structure
		char szFile[260];       // buffer for file name
		HWND hwnd;              // owner window
		HANDLE hf;              // file handle

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get()->GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Display the Open dialog box. 

		if (GetSaveFileName(&ofn) == TRUE)
			return std::string(ofn.lpstrFile);

		return std::string();
	}

#endif // PLATFORM_WINDOWS
}
