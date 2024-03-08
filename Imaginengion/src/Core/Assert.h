#pragma once

#include "Core/Core.h"
#include "Core/Log.h"

#include <filesystem>

#ifdef IMAGINE_ENABLE_ASSERTS
	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define IMAGINE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { IMAGINE##type##ERROR(msg, __VA_ARGS__); IMAGINE_DEBUGBREAK(); } }
	#define IMAGINE_INTERNAL_ASSERT_WITH_MSG(type, check, ...) IMAGINE_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define IMAGINE_INTERNAL_ASSERT_NO_MSG(type, check) IMAGINE_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", IMAGINE_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define IMAGINE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define IMAGINE_INTERNAL_ASSERT_GET_MACRO(...) IMAGINE_EXPAND_MACRO( IMAGINE_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, IMAGINE_INTERNAL_ASSERT_WITH_MSG, IMAGINE_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define IMAGINE_ASSERT(...) IMAGINE_EXPAND_MACRO( IMAGINE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define IMAGINE_CORE_ASSERT(...) IMAGINE_EXPAND_MACRO( IMAGINE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define IMAGINE_ASSERT(x, ...)
	#define IMAGINE_CORE_ASSERT(x, ...)
#endif