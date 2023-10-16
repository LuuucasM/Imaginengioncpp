#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace IM {

	Log::Log() {

	}

	Log::~Log() {

	}

	std::shared_ptr<spdlog::logger> CoreLogger;
	std::shared_ptr<spdlog::logger> ClientLogger;

	void Log::Init() {
		spdlog::set_pattern("%*[%T] %n: %v%$");
		CoreLogger = spdlog::stdout_color_mt("IMAGINE");
		CoreLogger->set_level(spdlog::level::trace);

		ClientLogger = spdlog::stdout_color_mt("APP");
		ClientLogger->set_level(spdlog::level::trace);

	}

}