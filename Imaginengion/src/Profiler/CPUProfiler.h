#pragma once

namespace IM {
	class CPUProfiler
	{
	public:
		CPUProfiler();
		~CPUProfiler() = default;
		uint32_t GetThreadCount() { return _ThreadCount; }
	private:
		uint32_t _ThreadCount;
	};
}

