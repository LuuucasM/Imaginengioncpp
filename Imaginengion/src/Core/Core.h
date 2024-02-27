#pragma once

#include <memory>

#include "PlatformDetection.h"

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

	template<typename T>
	using WeakPtr = std::weak_ptr<T>;
}