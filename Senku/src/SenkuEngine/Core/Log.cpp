#include "PrecompiledHeader.h"
#include "spdlog/sinks/stdout_color_sinks.h"


#include "Log.h"
namespace Senku
{

	Ref<spdlog::logger> Log::s_Logger;

	//Log::Log()
	//{
	//
	//}
	//
	//Log::~Log()
	//{
	//}

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_Logger = spdlog::stdout_color_mt("SenkuEngine");
		s_Logger->set_level(spdlog::level::trace);


	}
}