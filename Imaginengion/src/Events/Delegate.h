#pragma once

#include "Core/Core.h"
#include "Core/Log.h"
#include "Debug/PerfProfiler.h"

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

		/*
		* Adds a listener to this Delegate
		* @template T: The type of the object that contains the function this Delegate calls when Delegate calls braodcast
		* @param object: A reference to the object that contains the function this Delegate calls when this Delegate calls broadcast
		* @param func: A pointer to the member function of the given object
		*/
		template<class T>
		void AddListener(T *object, void (T::*func)(Args...)) {

			IMAGINE_PROFILE_FUNCTION();

			std::any listenerObj = object;
			std::function<void(Args...)> listenerFunc = [object, func](Args... args) {
				(object->*func)(args...);
			};
			listeners.emplace_back(listenerObj, listenerFunc);
		}
		/*
		* Removes a listener from the list of listeners on this Delegate
		* @param object: A reference to the object that we want to remove
		*/
		void RemoveListener(std::any object) {

			IMAGINE_PROFILE_FUNCTION();

			//remove specified object
			listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
				[object](const Listener& listener) {return listener.object == object; }),
				listeners.end());
		}
		/*
		* Broadcast to all listeners to activate their On Delegate function
		* @param args: the arguments that we will call each listeners Delegate function with
		*/
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

		/*
		* Helper functions to get the Delegate type and Delegate category
		* NOTE: Delegate type and Delegate category are currently unused in the program
		* but I am keeping it around in case I might need it later. I know this current
		* Delegate implmentation does not cover some cases in an ideal Delegate system so
		* until then I will keep this
		*/

	private:
		struct Listener {
			std::any object;
			std::function<void(Args...)> func;
		};

		std::vector<Listener> listeners;
	};
}