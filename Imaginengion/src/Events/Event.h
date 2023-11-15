#pragma once

#include "Core/Core.h"
#include "Core/Log.h"

#include <any>
#include <functional>
#include <vector>
#include <string>

namespace IM {
	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,

	};

	enum EventCategory {
		None = 0,
		EC_Application = BIT(0),
		EC_Input = BIT(1),
		EC_Keyboard = BIT(2),
		EC_Mouse = BIT(3),
		EC_MouseButton = BIT(4),

	};

	template<typename... Args>
	class Event {

	public:
		Event(EventType type, EventCategory category, std::string name)
			: event_type(type), event_category(category), event_name(name) { }
		~Event() { }

		/*
		* Adds a listener to this event
		* @template T: The type of the object that contains the function this event calls when event calls braodcast
		* @param object: A reference to the object that contains the function this event calls when this event calls broadcast
		* @param func: A pointer to the member function of the given object
		*/
		template<typename T>
		void AddListener(T *object, void (T::*func)(Args...)) {
			std::any listenerObj = object;
			std::function<void(Args...)> listenerFunc = [object, func](Args... args) {
				(object->*func)(args...);
			};
			listeners.emplace_back(listenerObj, listenerFunc);
		}
		/*
		* Removes a listener from the list of listeners on this event
		* @param object: A reference to the object that we want to remove
		*/
		void RemoveListener(std::any object) {
			//remove specified object
			listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
				[object](const Listener& listener) {return listener.object == object; }),
				listeners.end());
		}
		/*
		* Broadcast to all listeners to activate their OnXEvent function
		* @param args: the arguments that we will call each listeners OnXEvent function with
		*/
		void Broadcast(Args... args) {
			//remove any invalid objects first before calling each listener
			listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
				[](const Listener& listener) {return !listener.object.has_value(); }),
				listeners.end());

			for (auto const listener : listeners) {
				listener.func(args...);
			}
			IMAGINE_CORE_INFO("Event: {0} Args: ", event_name);
		}

		/*
		* Helper functions to get the event type and event category
		* NOTE: event type and event category are currently unused in the program
		* but I am keeping it around in case I might need it later. I know this current
		* event implmentation does not cover some cases in an ideal event system so
		* until then I will keep this
		*/
		inline EventType GetEventType() const {return event_type;}
		inline EventCategory GetEventCategory() const {return event_category;}	

	private:
		std::string event_name;
		EventType event_type;
		EventCategory event_category;

		struct Listener {
			std::any object;
			std::function<void(Args...)> func;
		};

		std::vector<Listener> listeners;
	};
}