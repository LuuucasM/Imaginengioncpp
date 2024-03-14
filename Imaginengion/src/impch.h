#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>

#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <any>

#include "Core/Core.h"
#include "Core/Log.h"
#include "Profiler/PerfProfiler.h"


#ifdef IMAGINE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif