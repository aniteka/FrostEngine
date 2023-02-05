#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>


#define CREATE_LOGGER(loggerName) \
	static auto loggerName = Core::Log::default_logger()->clone(#loggerName)

namespace Core::Log
{
	using namespace spdlog;
	namespace _defaultInit
	{
		class LogInit
		{
		public:
			LogInit()
			{
				std::vector<sink_ptr> sinks;
				sinks.push_back(std::make_shared<sinks::stdout_color_sink_mt>());
				sinks.push_back(std::make_shared<sinks::basic_file_sink<std::mutex>>("logfile"));
				auto logger = std::make_shared<Log::logger>("GlobalLogger", std::begin(sinks), std::end(sinks));
				set_default_logger(logger);
			}
		};
		static LogInit _init{};
	}
}

