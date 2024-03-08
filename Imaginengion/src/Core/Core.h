#pragma once

#include <memory>

#include "PlatformDetection.h"

#ifdef IMAGINE_DEBUG
	#ifdef IMAGINE_PLATFORM_WINDOWS
		#define IMAGINE_DEBUGBREAK() __debugbreak()
	#elif defined(IMAGINE_PLATFORM_LINUX)
		#include <signal.h>
		#define IMAGINE_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesnt support debugbreak yet!"
	#endif
	#define IMAGINE_ENABLE_ASSERTS
#endif

#define IMAGINE_EXPAND_MACRO(x) x
#define IMAGINE_STRINGIFY_MACRO(x) #x

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

	template<typename T>
	using WeakPtr = std::weak_ptr<T>;
}

#include "Core/Log.h"
#include "Core/Assert.h"