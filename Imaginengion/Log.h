#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace IM {
	class Log
	{
	private:
		static std::shared_ptr<spdlog::logger> CoreLogger;
		static std::shared_ptr<spdlog::logger> ClientLogger;
	public:
		Log();
		~Log();

		void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return ClientLogger; }
	};
}