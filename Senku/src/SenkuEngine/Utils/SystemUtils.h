#pragma once

namespace Senku
{

	class FileDialog
	{
	public:
		static std::string OpenFile(const char* filter = "All files (*.*)\0*.*\0"); //example "Image Files (*.png, *.jpg)\0*.png;*.jpg\0CPP Files (*.cpp, *.h)\0*.cpp;*.h\0"
		static std::string SaveFile(const char* filter = "All files (*.*)\0*.*\0");
	};
}