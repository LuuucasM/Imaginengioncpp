#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Renderer/RenderContext.h"

#include <string>
#include <functional>

namespace IM {
	struct WindowProps {
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		WindowProps(std::string title = "Imaginengion", uint32_t width = 1280, uint32_t height = 720)
			: Title(title), Width(width), Height(height){}

	};
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() = default;

		/*
		* Function to update the window. Called in application run loop
		*/
		virtual void OnUpdate() = 0;

		//Helper functions to get width and height of window
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		//Helper functions to set and get Vsync option
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		/*
		* Function to create the window
		*/
		static ScopePtr<Window> Create(const WindowProps& props = WindowProps());

		ScopePtr<RenderContext> _RenderContext = nullptr;
	};
}