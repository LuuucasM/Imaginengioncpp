#pragma once


#include "Core/Core.h"

#include <string>
#include <functional>

namespace IM {
	enum class EventType {
		None = 0,
		ET_WindowClose, ET_WindowResize, ET_WindowFocus, ET_WindowLostFocus, ET_WindowMoved,
		ET_KeyPressed, ET_KeyReleased, ET_KeyTyped,
		ET_MouseButtonPressed, ET_MouseButtonReleased, ET_MouseMoved, ET_MouseScrolled,
		ET_SceneChange
	};

	enum EventCategory {
		None = 0,
		EC_Application = BIT(0),
		EC_Input = BIT(1),
		EC_Keyboard = BIT(2),
		EC_Mouse = BIT(3),
		EC_MouseButton = BIT(4),
		EC_Editor = BIT(5)
	};

	class Event {
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }
		bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }

	public:
		bool _bHandled = false;
	};

	class EventDispatcher {
	public:
		EventDispatcher(Event& event)
			: _Event(event) {}
		template<typename T, typename F>
		bool Dispatch(const F& func) {
			if (_Event.GetEventType() == T::GetStaticType()) {
				_Event._bHandled |= func(static_cast<T&>(_Event));
				return true;
			}
			return false;
		}
	private:
		Event& _Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.ToString(); }

}