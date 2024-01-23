#pragma once

#include <memory>

#ifdef _WIN32
	#ifdef _WIN64
		#define IMAGINE_PLATFORM_WINDOWS
	#else
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define IMAGINE_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define IMAGINE_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#define "Unknown Apple platform!"
	#endif
#elif defined(__ANDRIOD__)
	#define IMAGINE_PLATFORM_ANDRIOD
	#error "Andriod is not supported!"
#elif defined(__linux__)
	#define IMAGINE_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#error "Unknown platform!"
#endif

#ifdef IMAGINE_DEBUG
	#define IMAGINE_ENABLE_ASSERTS
#endif

#ifdef IMAGINE_ENABLE_ASSERTS
	#define IMAGINE_ASSERT(x, ...) { if(!(x)) {IMAGINE_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
	#define IMAGINE_CORE_ASSERT(x, ...) { if(!(x)) {IMAGINE_CORE_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
#else
	#define IMAGINE_ASSERT(x, ...)
	#define IMAGINE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define IMAGINE_BIND_EVENT(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace IM {
	template<typename T>
	using ScopePtr = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr ScopePtr<T> CreateScopePtr(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
	template<typename T>
	using RefPtr = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr RefPtr<T> CreateRefPtr(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}