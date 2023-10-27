#include "impch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace IM {
	std::shared_ptr<spdlog::logger> Log::CoreLogger;
	std::shared_ptr<spdlog::logger> Log::ClientLogger;

	void Log::Init() {
		//set the pattern for outputting logger messages
		spdlog::set_pattern("%l [%D %T] %n: %v%$");

		//create and set level of loggers
		CoreLogger = spdlog::stdout_color_mt("ENGINE");
		CoreLogger->set_level(spdlog::level::trace);

		ClientLogger = spdlog::stdout_color_mt("APP");
		ClientLogger->set_level(spdlog::level::trace);

	}

}