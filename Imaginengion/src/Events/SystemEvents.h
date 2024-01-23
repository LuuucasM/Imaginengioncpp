#pragma once


#include "Event.h"
#include "Core/Keycodes.h"
#include "Core/MouseCodes.h"

#include <sstream>

namespace IM { 
	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(size_t width, size_t height)
			: _Width(width), _Height(height) {}

		size_t GetWidth() const { return _Width; }
		size_t GetHeight() const { return _Height; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << _Width << ", " << _Height;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::ET_WindowResize; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "WindowResize"; }
		int GetCategoryFlags() const override { return EC_Application; }
	private:
		size_t _Width, _Height;
	};

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() = default;

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowCloseEvent";
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::ET_WindowClose; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "WindowClose"; }
		int GetCategoryFlags() const override { return EC_Application; }
	};

	class KeyEvent : public Event {
	public:
		KeyCode GetKeyCode() const { return _KeyCode; }
		int GetCategoryFlags() const override { return EC_Keyboard | EC_Input; }
	protected:
		KeyEvent(KeyCode keycode)
			: _KeyCode(keycode) {}
		KeyCode _KeyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(KeyCode keycode, int repeatCount)
			: KeyEvent(keycode), _RepeatCount(repeatCount){}
		int GetRepeatCount() const { return _RepeatCount; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << _KeyCode << " (" << _RepeatCount << " repeats)";
			return ss.str();
		}
		static EventType GetStaticType() { return EventType::ET_KeyPressed; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "KeyPressed"; }
	private:
		int _RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << _KeyCode;
			return ss.str();
		}
		static EventType GetStaticType() { return EventType::ET_KeyReleased; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "KeyReleased"; }
	};

	class KeyTypeEvent : public KeyEvent {
	public:
		KeyTypeEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypeEvent: " << _KeyCode;
			return ss.str();
		}
		static EventType GetStaticType() { return EventType::ET_KeyTyped; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "KeyTyped"; }
	};

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(const float x, const float y)
			: _MouseX(x), _MouseY(y) {}

		std::pair<float, float> GetNewPos() const { return { _MouseX, _MouseY }; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << _MouseX << ", " << _MouseY;
			return ss.str();
		}
		static EventType GetStaticType() { return EventType::ET_MouseMoved; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "MouseMoved"; }
		int GetCategoryFlags() const override { return EC_Mouse | EC_Input; }
	private:
		float _MouseX, _MouseY;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset)
			: _XOffset(xOffset), _YOffset(yOffset) {}

		float GetXOffset() const { return _XOffset; }
		float GetYOffset() const { return _YOffset; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << _YOffset << ", " << _YOffset;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::ET_MouseScrolled; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "MouseScrolled"; }
		int GetCategoryFlags() const override { return EC_Mouse | EC_Input; }
	private:
		float _XOffset, _YOffset;
	};

	class MouseButtonEvent : public Event {
	public:
		MouseCode GetMouseButton() const { return _Button; }
		int GetCategoryFlags() const override { return EC_Mouse | EC_Input | EC_MouseButton; }
	protected:
		MouseButtonEvent(const MouseCode button)
			: _Button(button) {}
		MouseCode _Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << _Button;
			return ss.str();
		}
		static EventType GetStaticType() { return EventType::ET_MouseButtonPressed; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "MouseButtonPressed"; }
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << _Button;
			return ss.str();
		}
		static EventType GetStaticType() { return EventType::ET_MouseButtonReleased; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "MouseButtonReleased"; }
	};

}