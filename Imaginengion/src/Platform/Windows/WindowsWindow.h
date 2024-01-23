#pragma once

#include "Core/Window.h"

#include <GLFW/glfw3.h>

const int VSYNC_ENABLED = 1;
const int VSYNC_DISABLED = 0;

namespace IM {

	//class to hold a windows specific window
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		//called in the main application run loop 
		void OnUpdate() override;

		//helpers to get height and width of window using our data struct
		uint32_t GetWidth() const override { return _Data.Width; }
		uint32_t GetHeight() const override { return _Data.Height; }

		//Vsync functions
		void SetEventCallback(const EventCallbackFn& callback) override { _Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override { return _Data.VSync; };

		void* GetNativeWindow() const override { return _Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
		GLFWwindow* _Window;

		//WindowData will be used to pass into glfw Set User Pointer function
		struct WindowData {
			std::string Title;
			uint32_t Width, Height;
			bool VSync;
			
			EventCallbackFn EventCallback;
		};
		WindowData _Data;
	};
}