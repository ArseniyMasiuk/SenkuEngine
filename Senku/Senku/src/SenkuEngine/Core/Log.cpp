#include "PrecompiledHeader.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Senku
{

	std::shared_ptr<spdlog::logger> Log::s_Logger = nullptr;

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