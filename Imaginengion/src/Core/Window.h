#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Renderer/RenderContext.h"

#include <string>

namespace IM {
	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		WindowProps(std::string title = "Imaginengion", unsigned int width = 1280, unsigned int height = 720)
			: Title(title), Width(width), Height(height){}

	};
	class IMAGINE_API Window
	{
	public:
		Window() {};
		virtual ~Window() {};

		/*
		* Function to update the window. Called in application run loop
		*/
		virtual void OnUpdate() = 0;

		//Helper functions to get width and height of window
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//Helper functions to set and get Vsync option
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		/*
		* Function to create the window
		*/
		static Window* Create(const WindowProps& props = WindowProps());

		ScopePtr<RenderContext> _RenderContext = nullptr;

		/*
		* All of the events that can be made on the window
		*/
		Event<int, int> WindowResizeEvent{ EventType::WindowResize, EventCategory::EC_Application, "WindowResizeEvent"};
		Event<> WindowCloseEvent{ EventType::WindowClose, EventCategory::EC_Application, "WindowCloseEvent"};
		Event<int, int> KeyPressedEvent{ EventType::KeyPressed, EventCategory::EC_Keyboard, "KeyPressedEvent"};
		Event<int> KeyReleasedEvent{ EventType::KeyReleased, EventCategory::EC_Keyboard, "KeyReleasedEvent"};
		Event<int, int> KeyRepeatEvent{ EventType::KeyPressed, EventCategory::EC_Keyboard, "KeyRepeatEvent"};
		Event<int> MouseButtonPressedEvent{ EventType::MouseButtonPressed, EventCategory::EC_MouseButton, "MouseButtonPressedEvent"};
		Event<int> MouseButtonReleasedEvent{ EventType::MouseButtonReleased, EventCategory::EC_MouseButton, "MouseButtonReleasedEvent"};
		Event<float, float> MouseScrolledEvent{ EventType::MouseScrolled, EventCategory::EC_Mouse, "MouseScrolledEvent"};
		Event<float, float> MouseMovedEvent{ EventType::MouseMoved, EventCategory::EC_Mouse, "MouseMovedEvent"};
	};
}