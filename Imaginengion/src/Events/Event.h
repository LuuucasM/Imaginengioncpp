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

		template<typename T>
		void AddListener(T *object, void (T::*func)(Args...)) {
			std::any listenerObj = object;
			std::function<void(Args...)> listenerFunc = [object, func](Args... args) {
				(object->*func)(args...);
			};
			listeners.emplace_back(listenerObj, listenerFunc);
		}

		void RemoveListener(std::any object) {
			//remove specified object
			listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
				[object](const Listener& listener) {return listener.object == object; }),
				listeners.end());
		}

		void Broadcast(Args... args) {
			//remove any invalid objects first before calling each listener
			listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
				[](const Listener& listener) {return !listener.object.has_value(); }),
				listeners.end());
			for (auto const listener : listeners) {
				listener.func(args...);
			}
			IMAGINE_CORE_INFO("Invoked: {0} Args: ", event_name);
		}
		EventType GetEventType() const {
			return event_type;
		}
		EventCategory GetEventCategory() const {
			return event_category;
		}	

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