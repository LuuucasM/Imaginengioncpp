#pragma once

#include "Core/Window.h"
#include "Events/Event.h"

#include <GLFW/glfw3.h>
#include <string>

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
		inline unsigned int GetWidth() const override { return Data.Width; }
		inline unsigned int GetHeight() const override { return Data.Height; }

		//Vsync functions
		void SetVSync(bool enabled) override;
		bool IsVSync() const override { return Data.VSync; };

		inline void* GetNativeWindow() const override { return _Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
		GLFWwindow* _Window;

		//WindowData will be used to pass into glfw Set User Pointer function
		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			Window* _Window;
		};
		WindowData Data;
	};
}