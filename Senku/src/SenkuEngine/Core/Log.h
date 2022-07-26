#pragma once
#include "spdlog\spdlog.h"
#include "Base.h"
//#include <spdlog\fmt\bundled\format.h>
//#include <spdlog\fmt\ostr.h>
//#include <spdlog\fmt\xchar.h>

namespace Senku
{

	class Log
	{
	public:
		//Log();
		//~Log();
		static void Init();

		static Ref<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static Ref<spdlog::logger> s_Logger;
	};



}

//class some_class {};
//std::ostream& operator<<(std::ostream& os, const unsigned char* string)
//{
//	return os << reinterpret_cast<const char*>(string);
//}

//template<>
//struct fmt::formatter<unsigned char> {
//	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
//		return ctx.end();
//	}
//
//	template <typename FormatContext>
//	auto format(const unsigned char& input, FormatContext& ctx) -> decltype(ctx.out()) {
//		return format_to(ctx.out(),
//			"{}",
//			reinterpret_cast<const char*>(input));
//	}
//};

#define LOG_ERROR(...) Senku::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_WARN(...) Senku::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...) Senku::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...) Senku::Log::GetLogger()->trace(__VA_ARGS__)
//#define GAME_ENGINE_FATAL(...) Log::GetLogger()->fatal(__VA_ARGS__)