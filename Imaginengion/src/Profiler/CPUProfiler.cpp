#include "impch.h"
#include "CPUProfiler.h"

#include <thread>

namespace IM {
	CPUProfiler::CPUProfiler() {
		_ThreadCount = std::thread::hardware_concurrency();
	}
}