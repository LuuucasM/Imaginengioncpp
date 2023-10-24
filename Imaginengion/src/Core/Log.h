#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace IM {
	class IMAGINE_API Log
	{
	public:
		/*
		* Initialize the logging system. Must be called at start of main function
		*/
		static void Init();

		//retrieve the core and client loggers
		inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { return CoreLogger; }
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() { return ClientLogger; }
	private:
		//core and client logger instances for logging
		static std::shared_ptr<spdlog::logger> CoreLogger;
		static std::shared_ptr<spdlog::logger> ClientLogger;
	};
}

//core log macros
#define IMAGINE_CORE_CRITICAL(...) ::IM::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define IMAGINE_CORE_ERROR(...) ::IM::Log::GetCoreLogger()->error(__VA_ARGS__)
#define IMAGINE_CORE_WARN(...) ::IM::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define IMAGINE_CORE_INFO(...) ::IM::Log::GetCoreLogger()->info(__VA_ARGS__)
#define IMAGINE_CORE_TRACE(...) ::IM::Log::GetCoreLogger()->trace(__VA_ARGS__)

//client log macros
#define IMAGINE_CRITICAL(...) ::IM::Log::GetClientLogger()->critical(__VA_ARGS__)
#define IMAGINE_ERROR(...) ::IM::Log::GetClientLogger()->error(__VA_ARGS__)
#define IMAGINE_WARN(...) ::IM::Log::GetClientLogger()->warn(__VA_ARGS__)
#define IMAGINE_INFO(...) ::IM::Log::GetClientLogger()->info(__VA_ARGS__)
#define IMAGINE_TRACE(...) ::IM::Log::GetClientLogger()->trace(__VA_ARGS__)