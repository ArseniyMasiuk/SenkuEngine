#pragma once
#include "spdlog\spdlog.h"

namespace Senku
{

	class Log
	{
	public:
		//Log();
		//~Log();
		static void Init();

		inline static std::shared_ptr<spdlog::logger> GetLogger() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}

#define LOG_ERROR(...) Senku::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_WARN(...) Senku::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...) Senku::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...) Senku::Log::GetLogger()->trace(__VA_ARGS__)
//#define GAME_ENGINE_FATAL(...) Log::GetLogger()->fatal(__VA_ARGS__)