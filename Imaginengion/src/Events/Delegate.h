#pragma once

#include "Core/Core.h"
#include "Core/Log.h"
#include "Profiler/PerfProfiler.h"

#include <any>
#include <functional>
#include <vector>
#include <string>

namespace IM {
	template<typename... Args>
	class Delegate {

	public:
		Delegate() = default;
		~Delegate() = default;

		template<class T>
		void AddListener(T *object, void (T::*func)(Args...)) {

			IMAGINE_PROFILE_FUNCTION();

			std::any listenerObj = object;
			std::function<void(Args...)> listenerFunc = [object, func](Args... args) {
				(object->*func)(args...);
			};
			listeners.emplace_back(listenerObj, listenerFunc);
		}

		void RemoveListener(std::any object) {

			IMAGINE_PROFILE_FUNCTION();

			//remove specified object
			listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
				[object](const Listener& listener) {return listener.object == object; }),
				listeners.end());
		}

		void Broadcast(Args... args) {

			IMAGINE_PROFILE_FUNCTION();

			//remove any invalid objects first before calling each listener
			listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
				[](const Listener& listener) {return !listener.object.has_value(); }),
				listeners.end());

			for (auto const listener : listeners) {
				listener.func(args...);
			}
			//IMAGINE_CORE_INFO("Event: {} Args: ", event_name);
		}

	private:
		struct Listener {
			std::any object;
			std::function<void(Args...)> func;
		};

		std::vector<Listener> listeners;
	};
}